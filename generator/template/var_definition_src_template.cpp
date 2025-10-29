#include "../include/ast.h"
#include "../include/var_definition.h"
#include <algorithm>
//#define WEAK_TYPE
__IS_WEAK_TYPE__
using namespace std;
#define NOTEXISTS 0
#define DBG 0
shared_ptr<Scope> g_scope_current;
static shared_ptr<Scope> g_scope_root;
static map<int, shared_ptr<Scope>> scope_id_map;
static int g_scope;

static set<int> all_functions;
set<int> all_compound_types;
set<int> all_arrays;

map<TYPEID, map<int, TYPEID>> pointer_map; //original_type:<level: typeid>
static map<TYPEID, shared_ptr<VarType>> type_map;

static vector<string> basic_int_types = { __SEMANTIC_BASIC_INT_TYPES__ }; 
static vector<string> basic_float_types = { __SEMANTIC_BASIC_FLOAT_TYPES__ };

static map<string, shared_ptr<VarType>> basic_types;
set<TYPEID> basic_types_set; // For faster search

bool is_internal_obj_setup = false;
bool is_in_class = false;

// Function to check if a substring matches any type in the vector
bool is_basic_int_type(const string &str) {
    return find(basic_int_types.begin(), basic_int_types.end(), str) != basic_int_types.end();
}

string get_float_type_name(){
    return basic_float_types[rand() % basic_float_types.size()];
}
string get_int_type_name(){
    return basic_int_types[rand() % basic_int_types.size()];
}


TYPEID VarType::get_type_id() const{
    return type_id_;
}


shared_ptr<Scope> get_scope_root(){
    return g_scope_root;
}



bool is_derived_type(TYPEID dtype, TYPEID btype){
    auto derived_type = get_type_by_type_id(dtype);
    auto base_type = get_type_by_type_id(btype);

    if(base_type == derived_type) return true;

    assert(derived_type && base_type);    
    
    bool res = false;

    for(auto &t: derived_type->base_type_){
        assert(t->type_id_ != dtype);
        res = res || is_derived_type(t->type_id_, btype);
    }

    return res;
}

void init_basic_types(){

    vector<string> v_basic_types = {__SEMANTIC_BASIC_TYPES__};
    for(auto &line: v_basic_types){
        if(line.empty()) continue;
        auto new_id = gen_type_id();
        auto ptr = make_shared<VarType>();
        ptr->type_id_ = new_id;
        ptr->type_name_ = line;
        basic_types[line] = ptr;
        basic_types_set.insert(new_id);
        type_map[new_id] = ptr;
        //if(DBG) cout << "Basic types: " << line << " , type id: " << new_id << endl;
    }

    make_basic_type_add_map(ALLTYPES, "ALLTYPES");
    make_basic_type_add_map(ALLCOMPOUNDTYPE, "ALLCOMPOUNDTYPE");
    make_basic_type_add_map(ALLFUNCTION, "ALLFUNCTION");
    make_basic_type_add_map(ANYTYPE, "ANYTYPE");
    make_basic_type_add_map(ALLARRAYTYPES, "ALLARRAYTYPES");
    basic_types["ALLTYPES"] = get_type_by_type_id(ALLTYPES);
    basic_types["ALLCOMPOUNDTYPE"] = get_type_by_type_id(ALLCOMPOUNDTYPE);
    basic_types["ALLFUNCTION"] = get_type_by_type_id(ALLFUNCTION);
    basic_types["ANYTYPE"] = get_type_by_type_id(ANYTYPE);
    basic_types["ALLARRAYTYPES"] = get_type_by_type_id(ALLARRAYTYPES);
}


TYPEID get_basic_typeid_by_string(const string & s){
    if(basic_types.find(s) != basic_types.end()){
        return basic_types[s]->get_type_id();
    }

    return NOTEXISTS;
}

int gen_type_id(bool reset){
    static int id = 10;
    if (reset) {
        //id = 10;
        if(!type_map.empty()){
            //if(DBG) cout<<"resetting the type id"<<endl;
            id = type_map.rbegin()->first + 1;
        }
    }
    return id++;
}

void clear_type_map(){
    // clear type_map
    // copy basic type back to type_map
    pointer_map.clear();
    type_map.clear();
    all_compound_types.clear();
    all_arrays.clear();
    all_functions.clear();

    
    for(auto &i: basic_types){
        i.second->base_type_.clear();
        i.second->derived_type_.clear();
        type_map[i.second->get_type_id()] = i.second;
    }
    
    gen_type_id(true);

}

set<int> get_all_class(){
    set<int> res(all_compound_types);
    return res;
}

shared_ptr<Scope> get_scope_by_id(int scope_id){
    assert(scope_id_map.find(scope_id) != scope_id_map.end());
    return scope_id_map[scope_id];
}

shared_ptr<VarType> get_type_by_type_id(TYPEID type_id){
    if(type_map.find(type_id) != type_map.end())
        return type_map[type_id];
    return nullptr;
}

shared_ptr<CompoundType> get_compound_type_by_type_id(TYPEID type_id){
    if(type_map.find(type_id) != type_map.end()) 
        return static_pointer_cast<CompoundType>(type_map[type_id]);
    return nullptr;
}

shared_ptr<FunctionType> get_function_type_by_type_id(TYPEID type_id){
    if(type_map.find(type_id) != type_map.end()) 
        return static_pointer_cast<FunctionType>(type_map[type_id]);
    return nullptr;
}

shared_ptr<ArrayType> get_array_type_by_type_type_id(TYPEID type_id){
    if(type_map.find(type_id) != type_map.end()){
        return static_pointer_cast<ArrayType>(type_map[type_id]);
    }

    return nullptr;
}


bool is_array_type(TYPEID type_id){
    return all_arrays.find(type_id) != all_arrays.end();
}

bool is_compound_type(TYPEID type_id){
    return all_compound_types.find(type_id) != all_compound_types.end(); 
}

TYPEID get_compound_type_id_by_string(const string &s){
    for(auto k: all_compound_types){
        if(type_map[k]->type_name_ == s) return k;
    }
    return NOTEXISTS;
}

bool is_function_type(TYPEID type_id){
    return all_functions.find(type_id) != all_functions.end(); 
}

bool is_basic_type(TYPEID type_id){
    return basic_types_set.find(type_id) != basic_types_set.end();
}

bool is_basic_type(const string &s){
    return basic_types.find(s) != basic_types.end();
}

TYPEID get_basic_type_id_by_string(const string& s){
    if(s == "ALLTYPES") return ALLTYPES;
    if(s == "ALLCOMPOUNDTYPE") return ALLCOMPOUNDTYPE;
    if(s == "ALLFUNCTION") return ALLFUNCTION;
    if(s == "ANYTYPE") return ANYTYPE;
    if(is_basic_type(s) == false) return NOTEXISTS;
    return basic_types[s]->get_type_id();
}

TYPEID get_type_id_by_string(const string &s){
    for(auto iter: type_map){
        // if(DBG) cout<<"type_name_ " <<iter.second->type_name_<<endl;
        // if(DBG) cout<<"Looking for  " <<s<<endl;
        if(iter.second->type_name_ == s) return iter.first;
    }
    return NOTEXISTS;
}

shared_ptr<FunctionType> get_function_type_by_return_type_id(TYPEID type_id){
    int counter = 0;
    shared_ptr<FunctionType> result = nullptr;
    for(auto t: all_functions){
        shared_ptr<FunctionType> tmp_ptr = static_pointer_cast<FunctionType>(type_map[t]);
        if(tmp_ptr->return_type_ == type_id){
            if(result == nullptr){
                result = tmp_ptr;
            }else{
                if(rand() % (counter + 1) < 1){
                    result = tmp_ptr;
                }
            }
            counter ++;
        }
    }
    return nullptr;
}

void make_basic_type_add_map(TYPEID id, const string &s){
    auto res = make_basic_type(id, s);
    type_map[id] = res;
}

shared_ptr<VarType> make_basic_type(TYPEID id, const string &s){
    auto res = make_shared<VarType>();
    res->type_id_ = id;
    res->type_name_ = s;
     
    return res;
}

void forward_add_compound_type(string &structure_name){
    //if(DBG) cout << "pre define compound type" << endl;
    auto res = make_shared<CompoundType>();
    res->type_id_ = gen_type_id();
    res->type_name_ = structure_name;
    type_map[res->type_id_] = res;
    all_compound_types.insert(res->type_id_);
    auto all_compound_type = get_type_by_type_id(ALLCOMPOUNDTYPE);  
    assert(all_compound_type);
    all_compound_type->derived_type_.push_back(res);
    res->base_type_.push_back(all_compound_type);
}
shared_ptr<CompoundType> make_compound_type_by_scope(shared_ptr<Scope> scope, string& structure_name, unsigned long ir_id){
    shared_ptr<CompoundType> res = nullptr;

    auto tid = get_compound_type_id_by_string(structure_name);
    if(tid != NOTEXISTS) res = get_compound_type_by_type_id(tid);
    if(res == nullptr) {
        //if(DBG) cout << "Creating a compound type at scope: "<< scope->scope_id_ << endl; 
        res = make_shared<CompoundType>();
        res->type_id_ = gen_type_id();
        res->type_name_ = structure_name;
        //if(DBG) cout << "Make compound typeid:"<< res->type_id_ <<" Scope id:" << scope->scope_id_ << endl;
        
            all_compound_types.insert(res->type_id_); //here
            type_map[res->type_id_] = res; //here
            auto all_compound_type = get_type_by_type_id(ALLCOMPOUNDTYPE);  
            assert(all_compound_type);
            all_compound_type->derived_type_.push_back(res);
            res->base_type_.push_back(all_compound_type);
    }
    return res;
}
shared_ptr<CompoundType> add_vars_compound_type_by_scope(shared_ptr<Scope> scope, string& structure_name){
    shared_ptr<CompoundType> res = nullptr;

    auto tid = get_compound_type_id_by_string(structure_name);
    if(tid != NOTEXISTS) res = get_compound_type_by_type_id(tid);
    if(res == nullptr) {
        return res;
    }

    else {
        res->define_root_ = scope->v_ir_set_.back();
        //if(DBG) cout << "Got the compound type "<<res->type_id_<<endl;

        for(auto &defined_var: scope->m_defined_variables_){
            auto &var_type = defined_var.first;
            auto &var_names = defined_var.second;
            for(auto &var: var_names){
                auto &var_name = var.first;
                res->v_members_[var_type].push_back(var_name);
                //if(DBG) cout << "[struct member] add member: " << var_name << " type: " << var_type << " to structure " << res->type_name_ << endl;
            }
        }
    }
    
    return res;
}

shared_ptr<FunctionType> make_function_type(string &function_name, TYPEID return_type, vector<TYPEID> &args){
    auto res = make_shared<FunctionType>();
    res->type_id_ = gen_type_id();
    res->type_name_ = function_name;

    res->return_type_ = return_type;
    res->v_arg_types_ = args;
    
        type_map[res->type_id_] = res; //here
        all_functions.insert(res->type_id_); //here
        auto all_function_type = get_type_by_type_id(ALLFUNCTION); 
        all_function_type->derived_type_.push_back(res);
        res->base_type_.push_back(all_function_type);
    

    return res;
}

int make_array_type(string &array_name, TYPEID array_type, vector<int> &array_size){

    auto res = make_shared<ArrayType>();
    res->type_id_ = gen_type_id();
    res->type_name_ = array_name;

    res->array_base_type = array_type;
    res->array_size = array_size;

    type_map[res->type_id_] = res;
    all_arrays.insert(res->type_id_);

    auto all_array_type = get_type_by_type_id(ALLARRAYTYPES);
    all_array_type->derived_type_.push_back(res);
    res->base_type_.push_back(all_array_type);

    return res->type_id_;

}




void Scope::add_definition(int type, IR* ir){
        if(type == 0) return ;
        m_define_ir_[type].push_back(ir);
}


void Scope::add_definition(int type, const string &var_name, unsigned long id){
        if(type == 0) return ;
        m_defined_variables_[type].push_back(make_pair(var_name, id));
}

void Scope::add_definition(int type, const string &var_name, unsigned long id, ScopeType stype){
        if(type == 0) return ;
        m_defined_variables_[type].push_back(make_pair(var_name, id));
}

void mytest_debug(){
    //if(DBG) cout << "g_scope_root: " << g_scope_root->scope_id_ << endl;
    debug_scope_tree(g_scope_root);
}

TYPEID least_upper_common_type(TYPEID type1, TYPEID type2){

    if(type1 == type2){
        return type1;
    }

    if(is_derived_type(type1, type2)) return type2;
    else if(is_derived_type(type2, type1)) return type1;
    return NOTEXISTS;
}

TYPEID convert_to_real_type_id(TYPEID type1, TYPEID type2){

    if(type1 > ALLUPPERBOUND){
        return type1;
    }
    
    switch(type1){
        case ALLTYPES: { 
            if(type2 == ALLTYPES || type2 == NOTEXISTS){
 
                auto pick_ptr = random_pick(basic_types_set);
                return *pick_ptr;
            }
            else if(type2 == ALLCOMPOUNDTYPE){
                if(all_compound_types.empty()){
                    //if(DBG) cout << "empty me" << endl;
                    break;
                }
                auto res = random_pick(all_compound_types);
                return *res;
            }
            else if(is_basic_type(type2)){
                return type2;
            }
            else{
                auto parent_type_ptr = static_pointer_cast<CompoundType>(type_map[type2]);
                if(parent_type_ptr->v_members_.empty()){
                    break;
                }
                auto res = random_pick(parent_type_ptr->v_members_);
                return res->first;
            }
        }
        case ALLFUNCTION: {
            auto res = *random_pick(all_functions);
            return res;
        }

        case ALLCOMPOUNDTYPE: {
            if(type2 == ALLTYPES || type2 == ALLCOMPOUNDTYPE || is_basic_type(type2)|| type2 == NOTEXISTS){
                if(all_compound_types.empty()){
                    //if(DBG) cout << "empty me" << endl;
                    break;
                }
                auto res = random_pick(all_compound_types);
                return *res;
            }
            else{
                assert(is_compound_type(type2));
                auto parent_type_ptr = static_pointer_cast<CompoundType>(type_map[type2]);
                if(parent_type_ptr->v_members_.empty()){
                    break;
                }

                int counter = 0;
                auto res = NOTEXISTS;
                for(auto &iter: parent_type_ptr->v_members_){
                    if(is_compound_type(iter.first)){
                        if(rand() % (counter + 1) < 1){
                            res = iter.first;
                        }
                        counter ++;
                    }
                }
                return res;
            }
        }
    }
    return NOTEXISTS;
}

void VarType::printDetails(){
        cout<<"This is a simple VarType"<<endl;
        cout<<"Variable Typeid: "<<type_id_<<endl;
        cout<<"Variable typename: "<<type_name_<<endl;
    }

void FunctionType::printDetails(){
        cout<<"Function name: "<< type_name_ <<endl;
        cout<<"Function return type: "<< get_type_by_type_id(return_type_)->type_name_ <<endl;
        cout<<"Function arguemnt types: ";
        for(auto arg:v_arg_types_){
            cout<<arg<<"\t";
        }
        cout<<endl;
    }


void CompoundType::printDetails(){
        cout<<"CompoundType name: "<< type_name_ <<endl;
        
        cout<<"CompoundType variables types: ";
        
        for (const auto& kv : v_members_) {
            cout << "Type: " << kv.first << " - Variables: ";
            for (const string& name : kv.second) {
                cout << name << ", ";
            }
            cout << endl;
        }
        cout<<endl;
    }

void PointerType::printDetails(){
        cout<<"Pointer variable name: "<< type_name_ <<endl;
        cout<<"Pointer origin type: "<< orig_type_ <<endl;
        cout<<"Pointer basic type: "<< basic_type_ <<endl;
        cout<<"Pointer reference level: "<< reference_level_ <<endl;

    }

void ArrayType::printDetails(){
        cout<<"Array variable name: "<<type_name_<<endl;
        cout<<"Array base type: "<<array_base_type<<endl;
        cout<<"ArrayDimension: "<<array_size.size()<<endl;
        cout<<"ArraySizes: ";
        for(auto dim:array_size){
            cout<<dim<<"\t";
        }
        cout<<endl;
}

string CompoundType::get_member_by_type(TYPEID type){
    if(v_members_.find(type) == v_members_.end()){
        return "";
    }
    return *random_pick(v_members_[type]);
}

TYPEID CompoundType::get_random_member_type(){
    std::vector<int> keys;

    // Iterate over the map and extract the keys
    for (const auto& pair : v_members_) {
        keys.push_back(pair.first);
    }
    return *random_pick(keys);
}
void debug_scope_tree(shared_ptr<Scope> cur){

    if(cur == nullptr) return;
    for(auto &child: cur->children_){
        debug_scope_tree(child.second);
    }

    // if(DBG) cout << "------------------------" << endl;
    // if(DBG) cout << "Current_scope--> " << cur->scope_id_ << ":" << endl;
    // if(DBG) cout << "Definition set: " << endl;

    for(auto &iter :cur->m_defined_variables_){
        auto m_var_type = iter.first;
        //if(DBG) cout << "Type id:" << m_var_type << endl;
        auto m_var_def = get_type_by_type_id(m_var_type);
        if(m_var_def == nullptr) continue;    
    
        //Need to differentiate MyStruct vs variable of MyStruct
    
        m_var_def->printDetails();
        for(auto &name: iter.second) {
            //if(DBG) cout << "\t" << name.first << endl;
        }
    
    
        //if(DBG) cout << "_____________________________" << endl;
    }

    //if(DBG) cout << "------------------------" << endl;
}

void all_scopes_at_ir(shared_ptr<Scope> cur) {
    
    while(cur){
        // if(DBG) cout <<endl<< "~~~~AT SCOPE: " << cur->scope_id_ << endl;

        // if(DBG) cout << "------------------------" << endl;
        // if(DBG) cout << "Current_scope--> " << cur->scope_id_ << ":" << endl;
        // if(DBG) cout << "Definition set: " << endl;

        for(auto &iter :cur->m_defined_variables_){
            auto m_var_type = iter.first;
            //if(DBG) cout << "Type id:" << m_var_type << endl;
            auto m_var_def = get_type_by_type_id(m_var_type);
            if(m_var_def == nullptr) continue;    
        
           //Need to differentiate MyStruct vs variable of MyStruct
        
            m_var_def->printDetails();
            for(auto &name: iter.second) {
                //if(DBG) cout << "\t" << name.first << endl;
            }
        
        
       // if(DBG) cout << "_____________________________" << endl;
        }

    //if(DBG) cout << "------------------------" << endl;
    
        cur = cur->parent_.lock();
    }
}

void enter_scope(ScopeType scope_type){
    if(get_scope_translation_flag() == false) return;
    auto new_scope = gen_scope(scope_type);
    if(g_scope_root == NULL){
        //if(DBG) cout << "set g_scope_root, g_scope_current: " <<  new_scope << endl;
        g_scope_root = g_scope_current = new_scope;
    }else{
        new_scope->parent_ = g_scope_current;
        //if(DBG) cout << "use g_scope_current: " << g_scope_current << endl;
        g_scope_current->children_[new_scope->scope_id_] = new_scope;
        g_scope_current = new_scope;
        //if(DBG) cout << "[enter]change g_scope_current: " << g_scope_current << endl;
    }
   // if(DBG) cout << "Entering new scope: " << g_scope << endl;
}

void exit_scope(){
    if(get_scope_translation_flag() == false) return;
    //if(DBG) cout << "Exit scope: " << g_scope_current->scope_id_ << endl;
    g_scope_current = g_scope_current->parent_.lock();
    //if(DBG) cout << "[exit]change g_scope_current: " << g_scope_current << endl;
}

shared_ptr<Scope> gen_scope(ScopeType scope_type){
    g_scope ++ ;
    auto res = make_shared<Scope>(g_scope, scope_type);
    scope_id_map[g_scope] = res;
    return res;
}

void CompoundType::remove_unfix(IR* ir){
    can_be_fixed_ir_.erase(ir);
    v_members_[ir->value_type_].push_back(ir->str_val_);
}

string get_type_name_by_id(TYPEID type_id){
    if(type_id == 0) return "NOEXISTS";
    //if(DBG) cout << "get_type name: Type: " << type_id << endl;
    //if(DBG) cout << "get_type name: Str: " << type_map[type_id]->type_name_ << endl;

    if(type_map.count(type_id) > 0){
        return type_map[type_id]->type_name_;
    }

    return "NOEXIST";
}


int generate_pointer_type(int original_type, int pointer_level){
   
    //must be a positive level
    //if(DBG) cout<<"generating pointer type starts"<<endl;
    //if(DBG) cout<<"Input pointer original type "<< original_type<< " with level: "<<pointer_level<<endl;
    assert(pointer_level);
    if(pointer_map[original_type].count(pointer_level)) {
       // if(DBG) cout<<"pointer original type "<< original_type<< " found in pointer_map with level: "<<pointer_level<<endl;
        return pointer_map[original_type][pointer_level];
    } 

    auto cur_type = make_shared<PointerType>();
    cur_type->type_id_ = gen_type_id();
    cur_type->type_name_ = "pointer_typeid_" + std::to_string(cur_type->type_id_);
    
    int base_type = -1;
    if(pointer_level == 1)
        base_type = original_type;
    else
        base_type = generate_pointer_type(original_type, pointer_level-1);
    
    //if(DBG) cout<<"Calculated base type: "<< base_type<< " with level: "<<pointer_level<<endl;
    cur_type->orig_type_ = base_type;
    cur_type->reference_level_ = pointer_level;
    cur_type->basic_type_ = original_type;

    auto alltype_ptr = get_type_by_type_id(get_basic_type_id_by_string("ALLTYPES"));
    cur_type->base_type_.push_back(alltype_ptr);
    alltype_ptr->derived_type_.push_back(cur_type);

   // if(DBG) cout<<"type_map saving: "<< cur_type->type_id_<<endl;
    type_map[cur_type->type_id_] = cur_type;
    pointer_map[original_type][pointer_level] = cur_type->type_id_;
    debug_pointer_type(cur_type);
    return cur_type->type_id_;
}

int get_or_create_pointer_type(int type){
    bool is_found = false;
    int orig_type = -1;
    int level = -1;
    int res = -1;
    bool outter_flag = false;
    for(auto &each_orig: pointer_map){
        //if type is a basic type
        if(each_orig.first == type){
            is_found = true;
            level = 0;
            orig_type = type;
            break;
        }

        //not a basic type
        for(auto &each_level: each_orig.second){          
            if(each_level.second == type){
                is_found = true;
                level = each_level.first;
                orig_type = each_orig.first;
                outter_flag = true;
                break;
            }
        }
        if(outter_flag) break;
    }

    if(is_found == true){
        assert(level != -1 && orig_type != -1);

        //found target pointer type
        if(pointer_map[orig_type].count(level+1)) return pointer_map[orig_type][level+1];

        //found original type but no this level reference
        auto cur_type = make_shared<PointerType>();
        cur_type->type_id_ = gen_type_id();
        cur_type->type_name_ = "pointer_typeid_" + std::to_string(cur_type->type_id_);
        cur_type->orig_type_ = type;
        cur_type->reference_level_ = level + 1;
        cur_type->basic_type_ = orig_type;
        auto alltype_ptr = get_type_by_type_id(get_basic_type_id_by_string("ALLTYPES"));
        alltype_ptr->derived_type_.push_back(cur_type);
        cur_type->base_type_.push_back(alltype_ptr);

        type_map[cur_type->type_id_] = cur_type;
        pointer_map[orig_type][level+1] = cur_type->type_id_;
        res = cur_type->type_id_;
        debug_pointer_type(cur_type);
    }
    else{
        //new original type
        auto cur_type = make_shared<PointerType>();
        cur_type->type_id_ = gen_type_id();
        cur_type->type_name_ = "pointer_typeid_" + std::to_string(cur_type->type_id_);
        cur_type->orig_type_ = type;
        cur_type->reference_level_ = 1;
        cur_type->basic_type_ = type;
        auto alltype_ptr = get_type_by_type_id(get_basic_type_id_by_string("ALLTYPES"));
        alltype_ptr->derived_type_.push_back(cur_type);
        cur_type->base_type_.push_back(alltype_ptr);

        type_map[cur_type->type_id_] = cur_type;
        pointer_map[type][1] = cur_type->type_id_;
        res = cur_type->type_id_;
        debug_pointer_type(cur_type);
    }

    return res;
    
}

bool is_pointer_type(int type){
   // if(DBG) cout << "is_pointer_type: " << type << endl;
    auto type_ptr = get_type_by_type_id(type);
    return type_ptr->is_pointer_type();
}

shared_ptr<PointerType> get_pointer_type_by_type_id(TYPEID type_id){
    //if(type_map.find(type_id) == type_map.end()) return nullptr;
    
    auto res = static_pointer_cast<PointerType>(type_map[type_id]);
    //if(res->is_pointer_type() == false) return nullptr;

    return res;
}

void debug_pointer_type(shared_ptr<PointerType> &p){
    if(DBG) cout << "------new_pointer_type-------" << endl;
    if(DBG) cout << "type id: " << p->type_id_ << endl;
    if(DBG) cout << "orig_type: " << p->orig_type_ << endl;
    if(DBG) cout << "reference_level: " << p->reference_level_ << endl;
    if(DBG) cout << "basic_type: " << p->basic_type_ << endl;
    if(DBG) cout << "-----------------------------" << endl;
}

void reset_scope(){
    //if(DBG) cout << "call reset_scope" << endl;
    g_scope_current = NULL;
    g_scope_root = NULL;
    scope_id_map.clear();
    g_scope = 0;
}

void clear_definition_all(){
    clear_type_map();
    reset_scope();
}
