#ifndef __MUTATOR_H__
#define __MUTATOR_H__

#include <queue>
#include <set>
#include <map>

#define LUCKY_NUMBER 500

using namespace std;

enum RELATIONTYPE{
    kRelationElement,
    kRelationSubtype,
    kRelationAlias,
};

class Mutator{

public:
    Mutator(){
        srand(time(nullptr));
    }

    ~Mutator(){}

    IR * deep_copy_with_record(const IR * root, const IR * record);
    unsigned long hash(IR* );
    unsigned long hash(string &);

    IR * ir_random_generator(vector<IR *> v_ir_collector);

    vector<IR *> mutate_all(vector<IR*> &v_ir_collector); //done
    vector<IR*> mutate(IR* input); //done

    IR * strategy_insert(IR * cur); //Done
    IR * strategy_replace(IR * cur);  //done
    IR * strategy_replace_with_constraint(IR* cur);
    bool lucky_enough_to_be_mutated(unsigned int mutated_times); //done

    bool replace(IR* root, IR* old_ir, IR* new_ir); //done

    void init(string f_testcase = "", string f_common_string = "", string file2d = "", string file1d = "", string f_gen_type = "");//DONE

    void init_convertable_ir_type_map();
    void init_ir_library(string filename);//DONE
    void init_ir_library_from_a_file(string filename);

    void add_ir_to_library(IR*);//DONE
    void add_ir_to_library_limited(IR*);//DONE
    
    IR* get_ir_from_library(IRTYPE);//DONE
    void extract_struct(IR *); //Done

    vector<IR *> split_to_stmt(IR * root, map<IR**, IR*> &m_save, set<IRTYPE> &split_set);//done
    bool connect_back(map<IR**, IR*> &m_save); //done

 
    bool is_ir_type_connvertable(IRTYPE a, IRTYPE b);

    //void print_ir_library();
    void replace_x_with_random(IR *root);
 
    bool can_be_mutated(IR *);
    void add_ir_to_library_no_deepcopy(IR*);//DONE
    static const unsigned max_ir_library_size_ = 0x200;

    IR* record_ = NULL;
    IR* mutated_root_  = NULL;
    map<IRTYPE, vector<IR*>> ir_library_;
    map<IRTYPE, set<unsigned long>> ir_library_hash_;


    set<IRTYPE> string_types_;
    set<IRTYPE> int_types_;
    set<IRTYPE> float_types_;
    set<unsigned long> global_hash_;
    map<IRTYPE, set<IRTYPE>> m_convertable_map_;

};

unsigned int calc_node(IR * root);
bool contain_fixme(IR*);

void extract_struct_after_mutation(IR*);
#endif
