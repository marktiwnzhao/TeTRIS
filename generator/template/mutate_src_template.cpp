//#include <queue>
#include <set>
#include <map>
#include <iterator>
#include <assert.h>
#include <fstream>
#include <cstdio>
#include <climits>
#include <cfloat>
#include <algorithm>
#include <deque>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <random>
using namespace std;

#include "../include/ast.h"
#include "../include/define.h"
#include "../include/utils.h"
#include "../include/mutate.h"
#include "../include/typesystem.h"
#include "../include/var_definition.h"

#define _NON_REPLACE_


#define MUTATE_DBG 0

static inline bool not_unknown(IR* r){
    return r->type_ != kUnknown;
}

static inline bool is_leaf(IR* r){
    return r->left_ == NULL && r->right_ == NULL;
}

// Need No fix
IR * Mutator::deep_copy_with_record(const IR * root, const IR * record){
    IR * left = NULL, * right = NULL, * copy_res;

    if(root->left_) left = deep_copy_with_record(root->left_, record); // do you have a second version for deep_copy that accept only one argument?                                                  
    if(root->right_) right = deep_copy_with_record(root->right_, record);//no I forget to update here

    if(root->op_ != NULL)
        copy_res = new IR(root->type_, OP3(root->op_->prefix_, root->op_->middle_, root->op_->suffix_), 
                    left, right, root->float_val_, root->str_val_, root->name_, root->mutated_times_, root->scope_, root->data_flag_);
    else
        copy_res = new IR(root->type_, NULL, left, right, root->float_val_, root->str_val_, root->name_, root->mutated_times_, root->scope_, root->data_flag_);

 
    copy_res->data_type_ = root->data_type_;

    if(root == record && record != NULL){
        this->record_ = copy_res;
    }
    
    return copy_res;

}


vector<IR *> Mutator::mutate_all(vector<IR *> &v_ir_collector){
    vector<IR *> res;
    set<unsigned long> res_hash;
    IR * root = v_ir_collector[v_ir_collector.size() - 1];

    mutated_root_ = root;
    auto tmp_str = root->to_string();
    res_hash.insert(hash(tmp_str));

    //if(MUTATE_DBG) cout<<"root: "<< tmp_str<<endl;
    //if(MUTATE_DBG) cout<<"hash value of root: "<< hash(tmp_str)<<endl;
    
    int num_iterations = 1 + rand() % 4;
    for(int i=0; i<num_iterations; i++){
        IR* original_ir = deep_copy(root);
        res.push_back(original_ir);
    }
    
    //res.push_back(new_ir);


    

    int ir_number = 0;
    for(auto ir: v_ir_collector){

        //if(MUTATE_DBG) cout << "Going Mutation: " << get_string_by_nodetype(ir->type_)<< ir_number << endl;
        if(ir == root) {
            ir_number++;
            continue;
        }
        
        //#ifdef MUTATE_UNKNOWN
        //if(not_mutatable_types_.find(ir->type_) != not_mutatable_types_.end() || !can_be_mutated(ir)) continue;
        //#else
        //if(ir->type_ == kUnknown || not_mutatable_types_.find(ir->type_) != not_mutatable_types_.end() || is_leaf(ir) || !can_be_mutated(ir) ) {
        if(ir->type_ == kUnknown || is_leaf(ir) || !can_be_mutated(ir) ) {
            //if(MUTATE_DBG) cout<< "Can't be mutated this"<<endl;
            ir_number++;
            continue;
        }
        //#endif

       
        //if(MUTATE_DBG) cout << "Mutating type: " << get_string_by_nodetype(ir->type_)<< ir_number << endl;
        vector<IR *> v_mutated_ir = mutate(ir);
        
        // if(MUTATE_DBG){
        //     if(v_mutated_ir.size()==0) cout<<"No mutation here"<<endl;
        //     else {
        //         cout<<"Mutated IRs size: "<<v_mutated_ir.size()<<endl;
        //     } 
        // }
       

        for(auto i: v_mutated_ir){
            IR * new_ir_tree = deep_copy_with_record(root, ir);
            //if(MUTATE_DBG) cout << "NEW mutated type: " << get_string_by_nodetype(i->type_) << endl;

            replace(new_ir_tree, this->record_, i);

            
            IR* backup = deep_copy(new_ir_tree);
            extract_struct(new_ir_tree);
            string tmp = new_ir_tree->to_string();
      
            unsigned tmp_hash = hash(tmp);
            // if(MUTATE_DBG){
            //     cout<<"new_ir_tree: "<< tmp<<endl;
            //     cout<<"has value of new_ir_tree: "<< tmp_hash<<endl;
            // }
            
            if(res_hash.find(tmp_hash) != res_hash.end()){
                //if(MUTATE_DBG) cout << "discarded NEW mutated type: " << get_string_by_nodetype(i->type_) << endl;
                deep_delete(new_ir_tree);
                deep_delete(backup);
                continue;
            }

            res_hash.insert(tmp_hash);
            deep_delete(new_ir_tree);

            res.push_back(backup);
            //if(MUTATE_DBG) cout<<"Backup====> "<<backup->to_string()<<endl;
        }
        ir_number++;
    }

    return res;
}




void Mutator::add_ir_to_library(IR * cur){
    //if(MUTATE_DBG) cout<<" From add_ir_to_library: "<<get_string_by_nodetype(cur->type_)<<endl;
    
    extract_struct(cur);
    add_ir_to_library_limited(cur);

    return;
}

void Mutator::add_ir_to_library_limited(IR* cur){
    if(cur->left_) add_ir_to_library_limited(cur->left_);
    if(cur->right_) add_ir_to_library_limited(cur->right_);
    
    auto type = cur->type_;
    auto h = hash(cur);

    if(ir_library_hash_[type].find(h) != ir_library_hash_[type].end()){
        //if(MUTATE_DBG) cout<<"Not possible to add this "<< get_string_by_nodetype(cur->type_)<<endl;
        return;
    }
    else if(!can_be_mutated(cur)){
        //if(MUTATE_DBG) cout<<"No update on ir due to !can_be_mutated "<< get_string_by_nodetype(cur->type_)<<endl;
        return;
    }

    if(ir_library_[type].size() >= max_ir_library_size_){
        auto rand_idx = get_rand_int(ir_library_[type].size());
        auto removed_ir = ir_library_[type][rand_idx];
        ir_library_[type][rand_idx] = ir_library_[type].back();
        ir_library_[type].pop_back();
        auto removed_h = hash(removed_ir);
        deep_delete(removed_ir);
        ir_library_hash_[type].erase(removed_h);
    }
    //if(MUTATE_DBG) cout<<"Adding to IR "<< get_string_by_nodetype(cur->type_)<<endl;
    ir_library_[type].push_back(deep_copy(cur));
    ir_library_hash_[type].insert(h);

    
}

void Mutator::add_ir_to_library_no_deepcopy(IR * cur){

    auto type = cur->type_;
    auto h = hash(cur);
    if(find(ir_library_hash_[type].begin(), ir_library_hash_[type].end(), h) != ir_library_hash_[type].end())
        return;

    ir_library_hash_[type].insert(h);
    ir_library_[type].push_back(cur);

    if(cur->left_) add_ir_to_library_no_deepcopy(cur->left_);
    if(cur->right_) add_ir_to_library_no_deepcopy(cur->right_);

    return;
}

void Mutator::init_convertable_ir_type_map(){
    __INIT_CONVERTABLE_TYPE_MAP__
}


void Mutator::init_ir_library(string filename){
    ifstream input_file(filename);
    string line;

    cout << "[*] init ir_library: " << filename << endl;
    while(getline(input_file, line)){
        if(line.empty()) continue;
        auto p = parser(line);
        if(p == NULL) continue;
        
        vector<IR *> v_ir;
        auto res = p->translate(v_ir);
        p->deep_delete();
        p = NULL;
        
        add_ir_to_library(res);
        deep_delete(res);      
    }
    return;
}

void Mutator::init_ir_library_from_a_file(string filename){
    char content[0x4000] = {0};
    auto fd = open(filename.c_str(), 0);
    
    if (fd == -1) {
        cout << "Error opening file: " << strerror(errno) << endl;
        return;
    }

    ssize_t bytes_read = read(fd, content, 0x3fff);
    if (bytes_read == -1) {
        cout << "Error reading file: " << strerror(errno) << endl;
        close(fd);
        return;
    }
    
    close(fd);


    auto p = parser(content);
    if(p == NULL) {cout << "init " << filename << " failed" << endl; return;}
    vector<IR *> v_ir;
    auto res = p->translate(v_ir);
    //if(MUTATE_DBG) cout<<" From init_ir_library_from_a_file: "<<get_string_by_nodetype(res->type_)<<endl;
    p->deep_delete();
    p = NULL;

    add_ir_to_library(res);
    deep_delete(res);
    //if(MUTATE_DBG) cout << "init " << filename << " success" << endl; 
    return;
}

void Mutator::init(string f_testcase, string f_common_string, string file2d, string file1d, string f_gen_type){

    //init lib from multiple sql
    if(!f_testcase.empty()) init_ir_library(f_testcase);
    
    float_types_.insert({kFloatLiteral});
    int_types_.insert(kIntLiteral);
    string_types_.insert(kStringLiteral);

   
    init_convertable_ir_type_map();

    return;
}

//Need No Fix
vector<IR *> Mutator::mutate(IR * input){
    vector<IR *> res;
 
    if(!lucky_enough_to_be_mutated(input->mutated_times_)){
        assert(0);
        return res; // return a empty set if the IR is not mutated
    }

    // if(MUTATE_DBG){
    //     cout << ".......... Starting strategy_replace_with_constraint"<<endl;
    // }

    for(int i = 0; i < 0x6; i ++){
        auto tmp = strategy_replace_with_constraint(input);
        if(tmp != NULL){
            res.push_back(tmp);
            // if (MUTATE_DBG) {
            // cout << "Strategy Replace this with constraint: " << input->to_string() << endl;
            // cout << "With: " << tmp->to_string() << endl;
            // //getchar();
            // }
        }
    }

    // if(MUTATE_DBG){
    //     cout << ".......... Ending strategy_replace_with_constraint" << endl<<endl;
    // }


    // if(MUTATE_DBG){
    //     cout << ".......... Starting strategy_insert"<< endl;
    // }

    IR* tmp = NULL;

    tmp = strategy_insert(input);
    if(tmp != NULL){
        res.push_back(tmp);
        // if (MUTATE_DBG) {
        //     cout << "Strategy Insert this: " << input->to_string() << endl;
        //     cout << "With: " << tmp->to_string() << endl;
        //     //getchar();
        // }
    }

    // if(MUTATE_DBG){
    //     cout << ".......... Ending strategy_insert" << endl<<endl;
    // }

    // if(MUTATE_DBG){
    //     cout << ".......... Starting strategy_replace"<< endl;
    // }

    tmp = strategy_replace(input);
    if(tmp != NULL){
    res.push_back(tmp);
    //    if (MUTATE_DBG) {
    //         cout << "Strategy only Replace: " << input->to_string() << endl;
    //         cout << "With " << tmp->to_string() << endl;
    //         //getchar();
    //     }
    }

    // if(MUTATE_DBG){
    //     cout << ".......... Ending strategy_replace" << endl<<endl;
    // }

    input->mutated_times_ += res.size();
    for(auto i : res){
        if(i == NULL) continue;
        i->mutated_times_ = input->mutated_times_ ;
    }
    return res;
}

bool Mutator::replace(IR * root , IR* old_ir, IR* new_ir){ 
    auto parent_ir = locate_parent(root, old_ir);
    assert(parent_ir);
 
    if(parent_ir->left_ == old_ir) {
        deep_delete(old_ir);
        parent_ir->left_ = new_ir;
        return true;
    }
    else if(parent_ir->right_ == old_ir) {
        deep_delete(old_ir);
        parent_ir->right_ = new_ir;
        return true;
    }
    assert(0);
    return false;
}



IR * Mutator::strategy_insert(IR * cur){
    assert(cur);

    IR* res = NULL;
    auto parent_type = cur->type_;
    //if(MUTATE_DBG) cout << "In strategy_insert Parent Type: " << get_string_by_nodetype(parent_type) << endl;
    
    if(cur->right_ == NULL && cur->left_ != NULL && not_unknown(cur->left_)){
        auto left_type = cur->left_->type_;
        if(MUTATE_DBG) cout << "In strategy_insert based on Left child: " << get_string_by_nodetype(left_type) << endl;
        for(int k=0; k<4; k++){
            auto fetch_ir = get_ir_from_library(parent_type);
            if(MUTATE_DBG) cout <<"fetched ir of Parent type : "<< fetch_ir->print();
           
            if(fetch_ir->left_ != NULL && fetch_ir->left_->type_ == left_type && fetch_ir->right_ != NULL){
                res = deep_copy(cur);
                res->right_ = deep_copy(fetch_ir->right_);
                if(MUTATE_DBG) cout << "Succsessfully inserted the fetched ir's right into Parent's Right child"<<endl;
                return res;
            }
        }
    }
   
    else if(cur->right_ != NULL && cur->left_ == NULL && not_unknown(cur->right_)){

        auto right_type = cur->right_->type_;
      
        //if(MUTATE_DBG) cout << "In strategy_insert based on Right child: " << get_string_by_nodetype(right_type) << endl;
        for(int k=0; k<4; k++){
            auto fetch_ir = get_ir_from_library(parent_type);
            //if(MUTATE_DBG) cout <<"fetched ir of Parent Type: "<< fetch_ir->print();
            
            if(fetch_ir->right_ != NULL && fetch_ir->right_->type_ == right_type && fetch_ir->left_ != NULL){
                res = deep_copy(cur);
                res->left_ = deep_copy(fetch_ir->left_);
                //if(MUTATE_DBG) cout << "Succsessfully inserted the fetched ir's left into Parent's Left child"<<endl;
                return res;
            }
        }
    }
    else if(cur->left_ == NULL && cur->right_ == NULL){
        //if(MUTATE_DBG) cout << "In strategy_insert the current node is Leaf Node"<< endl;
        for(int k=0; k<4; k++){
            auto fetch_ir = get_ir_from_library(parent_type);
            //if(MUTATE_DBG) cout <<"fetched ir of Parent Type: "<< fetch_ir->print();
            
            if(fetch_ir->right_ != NULL && fetch_ir->left_ != NULL){
                res = deep_copy(cur);
                res->left_ = deep_copy(fetch_ir->left_);
                res->right_ = deep_copy(fetch_ir->right_);
                //if(MUTATE_DBG) cout << "Succsessfully inserted the fetched ir's both childreb into Parent's both children"<<endl;
                return res;
            }
        } 
    }

    return res;
}

bool Mutator::is_ir_type_connvertable(IRTYPE a, IRTYPE b){
    if(a == b) return true;
    if(m_convertable_map_.find(a) == m_convertable_map_.end()){
        return false;
    }
    if(m_convertable_map_[a].find(b) == m_convertable_map_[a].end()){
        return false;
    }

    return true;
}

IR * Mutator::strategy_replace_with_constraint(IR* cur){
    assert(cur);


    if(cur->op_ == NULL || (cur->op_->prefix_.empty() && cur->op_->middle_.empty() && cur->op_->suffix_.empty())){
        //if (MUTATE_DBG) cout << "strategy_replace_with_constraint not possible in Type: " << get_string_by_nodetype(cur->type_) << endl;
        return NULL;
    }
    
    IRTYPE replace_type = cur->type_;
    if(m_convertable_map_.find(replace_type) != m_convertable_map_.end()){
        replace_type = *(random_pick(m_convertable_map_[cur->type_]));
    }
    
   // if (MUTATE_DBG) cout << "Input Type: " << get_string_by_nodetype(cur->type_) << endl;

    auto res = get_ir_from_library(replace_type);

    //if (MUTATE_DBG) cout << "Possible replacement from ir  Type: " << get_string_by_nodetype(res->type_) << endl;

    if(res->left_ && !cur->left_ || cur->left_ && !res->left_ || res->right_ && !cur->right_ || cur->right_ && !res->right_){
       // if (MUTATE_DBG) cout << "res has a left (or right) child that cur doesn't have, or vice versa." <<endl;
        return NULL;
    }

    if(res->left_ && !is_ir_type_connvertable(res->left_->type_, cur->left_->type_)){
       // if (MUTATE_DBG) cout << "Not possible for inconvertable left" <<endl;
         return NULL;
    }

    if(res->right_ && !is_ir_type_connvertable(res->right_->type_, cur->right_->type_)){
        //if (MUTATE_DBG) cout << "Not possible for inconvertable right" <<endl;
        return NULL;
    }



    auto save_res_left = res->left_;
    auto save_res_right = res->right_;
    auto save_res = res;
    res->left_ = NULL;
    res->right_ = NULL;

    res = deep_copy(res);

    save_res->left_ = save_res_left;
    save_res->right_ = save_res_right;
   
    if(cur->left_){
        res->left_ = deep_copy(cur->left_);
    }
    if(cur->right_){
        res->right_ = deep_copy(cur->right_);
    }

    //if (MUTATE_DBG) cout << "New ir  Type: " << get_string_by_nodetype(res->type_) << endl;

    return res;
    
}

IR * Mutator::strategy_replace(IR * cur){
    assert(cur);

    MUTATESTART

    DOLEFT

    if (cur->left_ != NULL && not_unknown(cur->left_) && !is_leaf(cur->left_)) {

        // if (MUTATE_DBG)
        //     cout << "Case Left:To be replaced left ir  Type: " << get_string_by_nodetype(cur->left_->type_) << endl;

        bool left_pos = true;
        auto new_node = get_ir_from_library(cur->left_->type_);
        // if (MUTATE_DBG)
        //     cout << "Case Left: Response left child from ir_library Type: " << get_string_by_nodetype(new_node->type_) << endl;

        if (new_node && get_string_by_nodetype(new_node->type_) == "StringLiteral" && (new_node->str_val_ == "")) {
            // if (MUTATE_DBG)
            //     cout << "Case Left:In left child an empty string from ir_library" << endl;
            // //return res;
            left_pos = false;
        }

        if (cur->left_->op_ != NULL && new_node->op_ != NULL) {
            if ((cur->left_->op_->prefix_ == "return") && (new_node->op_->prefix_ != "return")) {
                // if (MUTATE_DBG)
                //     cout << "Case Left:Catching mismatch for left child in return" << endl;
                //return res;
                left_pos = false;
            }
        }

        if (cur->left_->op_ != NULL && new_node->op_ != NULL) {
            if ((new_node->op_->middle_ == "=") && (cur->left_->op_->middle_ != "=")  ) {
                // if (MUTATE_DBG)
                //     cout << "Case Left:Catching mismatch for left child in double assignment" << endl;
                // return res;
                left_pos = false;
            }
        }
        
        if(left_pos){
            res = deep_copy(cur);

            deep_delete(res->left_);
            // if (MUTATE_DBG) {
            //     cout << "Case Left:In strategy_replace Replacing left: " << cur->left_->to_string() << endl;
            //     cout << "With: " << new_node->to_string() << endl;
            //     // cout << "New Type: " << get_string_by_nodetype(new_node->type_) << endl;
            // }
            if (new_node) replace_x_with_random(new_node);

            res->left_ = deep_copy(new_node);

        }
        
    }

    DORIGHT
#ifdef MUTATE_UNKNOWN
    if (cur->right_ != NULL) {
#else
    if (cur->right_ != NULL && not_unknown(cur->right_) && !is_leaf(cur->right_)) {
#endif
        // if (MUTATE_DBG)
        //     cout << "Case Right:To be replaced right ir  Type: " << get_string_by_nodetype(cur->right_->type_) << endl;

        bool right_pos = true;

        auto new_node = get_ir_from_library(cur->right_->type_);
        // if (MUTATE_DBG)
        //     cout << "Case Right:Response right child from ir_library Type: " << get_string_by_nodetype(new_node->type_) << endl;

        if (new_node && get_string_by_nodetype(new_node->type_) == "StringLiteral" && (new_node->str_val_ == "")) {
            // if (MUTATE_DBG)
            //     cout << "Case Right:In right child an empty string from ir_library" << endl;
            //return res;
            right_pos = false;
        }

        if (cur->right_->op_ != NULL && new_node->op_ != NULL) {
            if ((cur->right_->op_->prefix_ == "return") && (new_node->op_->prefix_ != "return")) {
                // if (MUTATE_DBG)
                //     cout << "Case Right:Catching mismatch for right child in return" << endl;
                //return res;
                right_pos = false;
            }
        }

        if (cur->right_->op_ != NULL && new_node->op_ != NULL) {
            if ((new_node->op_->middle_ == "=") && (cur->right_->op_->middle_ != "=")  ) {
                // if (MUTATE_DBG)
                //     cout << "Case Left:Catching mismatch for right child in double assignment" << endl;
                // return res;
                right_pos = false;
            }
        }

        
        if(right_pos){
            res = deep_copy(cur);

            // if (MUTATE_DBG) {
            //     cout << "Case Right:In strategy_replace Replacing right: " << cur->right_->to_string() << endl;
            //     cout << "With: " << new_node->to_string() << endl;
            //     // cout << "New Type: " << get_string_by_nodetype(new_node->type_) << endl;
            // }
            deep_delete(res->right_);
            if (new_node) replace_x_with_random(new_node);
            res->right_ = deep_copy(new_node);
        }
        
    }
    DOBOTH
    if (cur->left_ != NULL && cur->right_ != NULL && not_unknown(cur->left_) && not_unknown(cur->right_) && !is_leaf(cur->left_) && !is_leaf(cur->right_)) {

        // if (MUTATE_DBG)
        //     cout << "Case Both:Trying strategy_replace on Parent:" << get_string_by_nodetype(cur->type_) << endl;
        
        bool left_pos = true, right_pos = true;

        // if (MUTATE_DBG)
        //     cout << "Left node to be replaced: " << get_string_by_nodetype(cur->left_->type_) << endl;
        auto new_left = get_ir_from_library(cur->left_->type_);
        // if (MUTATE_DBG)
        //     cout << "Prosprective Left node from ir_library: " << get_string_by_nodetype(new_left->type_) << endl;

        if (new_left && get_string_by_nodetype(new_left->type_) == "StringLiteral" && (new_left->str_val_ == "")) {
            // if (MUTATE_DBG)
            //     cout << "Case Both:In left child an empty string from ir_library" << endl;
            left_pos = false;
            //return res;
        }

        if (cur->left_->op_ != NULL && new_left->op_ != NULL) {
            if ((cur->left_->op_->prefix_ == "return") && (new_left->op_->prefix_ != "return")) {
                // if (MUTATE_DBG)
                //     cout << "Case Both:Catching mismatch for left child in return" << endl;
                //return res;
                left_pos = false;
            }
        }

        if (cur->left_->op_ != NULL && new_left->op_ != NULL) {
            if ((new_left->op_->middle_ == "=") && (cur->left_->op_->middle_ != "=")  ) {
                // if (MUTATE_DBG)
                //     cout << "Case Left:Catching mismatch for left child in double assignment" << endl;
                // return res;
                left_pos = false;
            }
        }


        // if (MUTATE_DBG)
        //     cout << "Right node to be replaced: " << get_string_by_nodetype(cur->right_->type_) << endl;
        
        auto new_right = get_ir_from_library(cur->right_->type_);
        // if (MUTATE_DBG)
        //     cout << "Prosprective Right node from ir_library: " << get_string_by_nodetype(new_right->type_) << endl;
        
        if (new_right && get_string_by_nodetype(new_right->type_) == "StringLiteral" && (new_right->str_val_ == "")) {
            // if (MUTATE_DBG)
            //     cout << "Case Both:In right child an empty string from ir_library" << endl;
            right_pos = false;
            //return res;
        }
        if (cur->right_->op_ != NULL && new_right->op_ != NULL) {
            if ((cur->right_->op_->prefix_ == "return") && (new_right->op_->prefix_ != "return")) {
                // if (MUTATE_DBG)
                //     cout << "Case Both:Catching mismatch for right child in return" << endl;
                //return res;
                right_pos = false;
            }
        }

        if (cur->right_->op_ != NULL && new_right->op_ != NULL) {
            if ((new_right->op_->middle_ == "=") && (cur->right_->op_->middle_ != "=")  ) {
                // if (MUTATE_DBG)
                //     cout << "Case Both:Catching mismatch for right child in double assignment" << endl;
                // return res;
                right_pos = false;
            }
        }

        

        if(!left_pos && !right_pos){
            return res;
        }
        res = deep_copy(cur);

        if(right_pos){
            deep_delete(res->right_);
            if (new_right) replace_x_with_random(new_right);
            res->right_ = deep_copy(new_right);
            // if (MUTATE_DBG) {
            //     cout << "Case Both:In strategy_replace Replacing right: " << cur->right_->to_string() << endl;
            //     cout << "With: " << new_right->to_string() << endl;
            // }

        }
        
        if(left_pos){
            deep_delete(res->left_);
            if (new_left) replace_x_with_random(new_left);
            res->left_ = deep_copy(new_left);
            // if (MUTATE_DBG) {
            //     cout << "Case Both:In strategy_replace Replacing left: " << cur->left_->to_string() << endl;
            //     cout << "With: " << new_left->to_string() << endl;
            // }
        }
 
    }

    MUTATEEND

    //return res;
}

//Need No Fix
bool Mutator::lucky_enough_to_be_mutated(unsigned int mutated_times){
    if(get_rand_int(mutated_times+1) < LUCKY_NUMBER){
        return true;
    }
    return false;
}



// Fix, if no item, generate from scratch
IR* Mutator::get_ir_from_library(IRTYPE type){
    
    const int generate_prop = 1;
    const int threshold = 0;
    static IR* empty_ir = new IR(kStringLiteral, "");
  
    if (ir_library_[type].empty()) {
        if(MUTATE_DBG) cout<<"Empty IR returned from library"<<endl;
        return empty_ir;
    }
    if(MUTATE_DBG) cout << "Get this type from ir_library: " << get_string_by_nodetype(type) << endl;
    //IR *res=deep_copy(vector_rand_ele(ir_library_[type]));  //this is a potential memory lekage!!!!
    //return res;
    return vector_rand_ele(ir_library_[type]);
}


unsigned long Mutator::hash(string &sql){ 
    return mytest_hash(sql.c_str(), sql.size());
}


unsigned long Mutator::hash(IR * root){
    auto tmp_str = move(root->to_string());
    return this->hash(tmp_str);
}

//[modify starts]
// void Mutator::print_ir_library() {
//     cout << ir_library_.size() << endl;
//     for (auto &i : ir_library_) {
//         cout << "=========================================" << endl;

//         cout << get_string_by_nodetype(i.first) << ": " << i.second.size() << endl;
//         for (auto &j : i.second) {
//             cout << "**" << endl;
//             cout << j->print();
//             cout << j->to_string();
//             cout << "**" << endl;
//         }

//         cout << "=========================================" << endl;
//     }
// }

unsigned int calc_node(IR * root){
    unsigned int res = 0;
    if(root->left_) res += calc_node(root->left_);
    if(root->right_) res += calc_node(root->right_);

    return res + 1;
}


size_t minimum = 1;
size_t maximum = 5;

size_t getRandomNumber(size_t min_, size_t max_) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min_, max_);

    return dis(gen);
}
string generateRandomString(size_t length) {
    const string characters = "abcdefghijklmnopqrstuvwyzABCDEFGHIJKLMNOPQRSTUVWYZ";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, characters.size() - 1);

    string randomString;
    for (size_t i = 0; i < length; ++i) {
        randomString += characters[dis(gen)];
    }

    return randomString;
}
void Mutator::replace_x_with_random(IR *root) {
    if (root->left_)
        replace_x_with_random(root->left_);

    if (root->right_)
        replace_x_with_random(root->right_);

    if (root->left_ || root->right_)
        return;

    if (root->data_type_ != kDataWhatever) {
        // if (MUTATE_DBG)
        //     cout << "There should be an x: " << get_string_by_nodetype(root->type_) << endl;
        if (root->str_val_ == "x") {
            //if (MUTATE_DBG) cout << "Got an x" << endl;
            size_t randomValue = getRandomNumber(minimum, maximum);
            string randomReplacement = generateRandomString(randomValue);
            root->str_val_ = randomReplacement;
        }
        return;
    }
}




void extract_struct_after_mutation(IR* root){

    if(root->left_){
        if(root->left_->data_type_ == kDataFixUnit){
            if(contain_fixme(root->left_)){
                auto save_ir_id = root->left_->id_;
                auto save_scope = root->left_->scope_id_;
                deep_delete(root->left_);
                root->left_ = new IR(kStringLiteral, "FIXME");
                root->left_->scope_id_ = save_scope;
                root->left_->id_ = save_ir_id;
            }
        }else{
            extract_struct_after_mutation(root->left_);
        }
    }
    if(root->right_){
        if(root->right_->data_type_ == kDataFixUnit){
            if(contain_fixme(root->right_)){
                auto save_ir_id = root->right_->id_;
                auto save_scope = root->right_->scope_id_;
                deep_delete(root->right_);
                root->right_ = new IR(kStringLiteral, "FIXME");          
                root->right_->scope_id_ = save_scope;
                root->right_->id_ = save_ir_id;
            }
        }
        else{
            extract_struct_after_mutation(root->right_);
        }
    }
    return;
}
void Mutator::extract_struct(IR * root){
    auto type = root->type_;

    //if(MUTATE_DBG) cout<<" From extract_struct: "<<get_string_by_nodetype(root->type_)<<endl;

    if(root->left_){
        if(root->left_->data_type_ == kDataFixUnit){
            deep_delete(root->left_);
            //if(MUTATE_DBG) cout<<"Has kDataFixUnit: "<<get_string_by_nodetype(root->left_->type_)<<endl;
            root->left_ = new IR(kStringLiteral, "FIXME");          
        }else{
            extract_struct(root->left_);
        }
    }
    if(root->right_){
        if(root->right_->data_type_ == kDataFixUnit){
            deep_delete(root->right_);
            //if(MUTATE_DBG) cout<<"Has kDataFixUnit: "<<get_string_by_nodetype(root->right_->type_)<<endl;
            root->right_ = new IR(kStringLiteral, "FIXME");
        }
        else{
            extract_struct(root->right_);
        }
    }

    if(root->left_ || root->right_) return;
    
    if(root->data_type_ != kDataWhatever){
        //For identifiers i guess
        //if(MUTATE_DBG) cout<<"Leaves Don't have kDataWhatever: "<<get_string_by_nodetype(type)<<endl;
        root->str_val_ = "x"; 
        return ;     
    }
    
    if(string_types_.find(type) != string_types_.end()){
        //StringLiteral
        //if(MUTATE_DBG) cout<<"The type here is kStringLiteral: "<<get_string_by_nodetype(type)<<endl;
        root->str_val_ = "'x'";
    }else if(int_types_.find(type) != int_types_.end()){
        //IntLiteral 
        //UnaryExpression comes here and becomes 1 if it contains int values
        //if(MUTATE_DBG) cout<<" Don't Know in extract_struct int_types_:"<<get_string_by_nodetype(type)<<endl;
        root->int_val_ = 1;
    }else if(float_types_.find(type) != float_types_.end()){
        //FloatLiteral
        //UnaryExpression comes here and becomes 1.0 if it contains float values
        //if(MUTATE_DBG) cout<<" Don't Know in extract_struct float_types_: "<<get_string_by_nodetype(type)<<endl;
        root->float_val_ = 1.0;
    }
}



vector<IR *> Mutator::split_to_stmt(IR * root, map<IR**, IR*> &m_save, set<IRTYPE> &split_set){
    vector<IR *> res;
    deque<IR *> bfs = {root};
    
    while(!bfs.empty()){
        auto node = bfs.front();
        bfs.pop_front();

    
        if(node && node->left_) bfs.push_back(node->left_);
        if(node && node->right_) bfs.push_back(node->right_);

        if(node->left_ && find(split_set.begin(), split_set.end(), node->left_->type_) != split_set.end()){
            res.push_back(node->left_);
            m_save[&node->left_] = node->left_;
            node->left_ = NULL;
        }
        if(node->right_ && find(split_set.begin(), split_set.end(), node->right_->type_) != split_set.end()){
            res.push_back(node->right_);
            m_save[&node->right_] = node->right_;
            node->right_ = NULL;
        }        
    }
    if(find(split_set.begin(), split_set.end(), root->type_) != split_set.end())
        res.push_back(root);

    return res;
}


bool Mutator::connect_back(map<IR**, IR*> &m_save){
    for(auto &iter: m_save){
        *(iter.first) = iter.second;
    }
    return true;
}

static set<IR*> visited;


bool Mutator::can_be_mutated(IR * cur){
    bool res = true;
    if( cur->data_type_ == kDataVarDefine 
        || isDefine(cur->data_flag_) 
        || cur->data_type_ == kDataVarType 
        || cur->data_type_ == kDataClassType
        || cur->data_type_ == kDataLabel
        || cur->data_type_ == kDataGoToLabel ){
        return false;
    }
    if(cur->left_) res = res && can_be_mutated(cur->left_);
    if(cur->right_) res = res && can_be_mutated(cur->right_);
    return res;
}

bool contain_fixme(IR* ir){
    bool res = false;
    if(ir->left_ || ir->right_){
        if(ir->left_){
            res = res || contain_fixme(ir->left_);
        } 
        if(ir->right_){
            res = res || contain_fixme(ir->right_);
        }
        return res;
    }

    if(!ir->str_val_.empty() && ir->str_val_ == "FIXME"){
        return true;
    }

    return false;
}


