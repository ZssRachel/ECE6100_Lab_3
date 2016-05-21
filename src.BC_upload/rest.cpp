#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "rest.h"

extern int32_t NUM_REST_ENTRIES;

/////////////////////////////////////////////////////////////
// Init function initializes the Reservation Station
/////////////////////////////////////////////////////////////

REST* REST_init(void){
  int ii;
  REST *t = (REST *) calloc (1, sizeof (REST));
  for(ii=0; ii<MAX_REST_ENTRIES; ii++){
    t->REST_Entries[ii].valid=false;
  }
  assert(NUM_REST_ENTRIES<=MAX_REST_ENTRIES);
  return t;
}

////////////////////////////////////////////////////////////
// Print State
/////////////////////////////////////////////////////////////
void REST_print_state(REST *t){
 int ii = 0;
  printf("Printing REST \n");
  printf("Entry  Inst Num  S1_tag S1_ready S2_tag S2_ready  Vld Scheduled\n");
  for(ii = 0; ii < NUM_REST_ENTRIES; ii++) {
    printf("%5d ::  \t\t%d\t", ii, (int)t->REST_Entries[ii].inst.inst_num);
    printf("%5d\t\t", t->REST_Entries[ii].inst.src1_tag);
    printf("%5d\t\t", t->REST_Entries[ii].inst.src1_ready);
    printf("%5d\t\t", t->REST_Entries[ii].inst.src2_tag);
    printf("%5d\t\t", t->REST_Entries[ii].inst.src2_ready);
    printf("%5d\t\t", t->REST_Entries[ii].valid);
    printf("%5d\n", t->REST_Entries[ii].scheduled);
    }
  printf("\n");
}

/////////////////////////////////////////////////////////////
//------- DO NOT CHANGE THE CODE ABOVE THIS LINE -----------
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
// If space return true else return false
/////////////////////////////////////////////////////////////

bool  REST_check_space(REST *t){
    
    int ii = 0;
    
    for(ii = 0; ii < NUM_REST_ENTRIES; ii++)
        if(t->REST_Entries[ii].valid == false)
            return true;
    
    return false;
    
}

/////////////////////////////////////////////////////////////
// Insert an inst in REST, must do check_space first
/////////////////////////////////////////////////////////////

void  REST_insert(REST *t, Inst_Info inst){
    
    int ii = 0;
    
    for(ii = 0; ii < NUM_REST_ENTRIES; ii++){
        if(t->REST_Entries[ii].valid == false){

            t->REST_Entries[ii].inst = inst;
            t->REST_Entries[ii].valid = true;
            t->REST_Entries[ii].scheduled = false;
#ifdef DEBUG_LOG
            printf("REST_insert inst. %ld\n", t->REST_Entries[ii].inst.inst_num);
#endif
            break;
        }
    }
    
    assert(1);

}

/////////////////////////////////////////////////////////////
// When instruction finishes execution, remove from REST
/////////////////////////////////////////////////////////////

void  REST_remove(REST *t, Inst_Info inst){
    
    int ii = 0;
    
    for(ii = 0; ii < NUM_REST_ENTRIES; ii++){
        if((t->REST_Entries[ii].valid == true) && (t->REST_Entries[ii].scheduled == true)){
        
            if(t->REST_Entries[ii].inst.inst_num == inst.inst_num){
                
                t->REST_Entries[ii].valid = false;
                t->REST_Entries[ii].scheduled = false;

#ifdef DEBUG_LOG
                printf("REST_remove inst. %ld\n", t->REST_Entries[ii].inst.inst_num);
#endif
                
                break;
            }
        }
    }

}

/////////////////////////////////////////////////////////////
// For broadcast of freshly ready tags, wakeup waiting inst
/////////////////////////////////////////////////////////////

void  REST_wakeup(REST *t, int tag){

    int ii = 0;
    
    for(ii = 0; ii<NUM_REST_ENTRIES; ii++){
        if(t->REST_Entries[ii].valid == true){
            if(t->REST_Entries[ii].inst.src1_tag == tag){
                t->REST_Entries[ii].inst.src1_ready = true;
#ifdef DEBUG_LOG
                printf("REST_wakeup inst. %ld, tag: %d\n", t->REST_Entries[ii].inst.inst_num, t->REST_Entries[ii].inst.src1_tag);
#endif
            }
            if(t->REST_Entries[ii].inst.src2_tag == tag){
                t->REST_Entries[ii].inst.src2_ready = true;
#ifdef DEBUG_LOG
                printf("REST_wakeup inst. %ld, tag: %d\n", t->REST_Entries[ii].inst.inst_num, t->REST_Entries[ii].inst.src2_tag);
#endif
            }
        }
            
    }
    
}

/////////////////////////////////////////////////////////////
// When an instruction gets scheduled, mark REST entry as such
/////////////////////////////////////////////////////////////

void  REST_schedule(REST *t, Inst_Info inst){
    
    int ii = 0;
    
    for(ii = 0; ii < NUM_REST_ENTRIES; ii++){
        if(t->REST_Entries[ii].valid == true){
            
            if(t->REST_Entries[ii].inst.inst_num == inst.inst_num){
                
                t->REST_Entries[ii].scheduled = true;
                break;
            }
        }
    }
    
    assert(1);

}

bool REST_in_order_get_oldest(REST *t, Inst_Info *SC_inst){
    
    int ii, oldest_index = -1;
    uint64_t oldest_inst = 0xFFFFFFFFFFFFFFFF;
    
    for(ii = 0; ii < NUM_REST_ENTRIES; ii++){
        
        if((t->REST_Entries[ii].valid == true) && (t->REST_Entries[ii].scheduled == false)){
            if(t->REST_Entries[ii].inst.inst_num < oldest_inst){
                oldest_inst = t->REST_Entries[ii].inst.inst_num;
                oldest_index = ii;
            }
        }
    }
    
    // break the SC_latch loop if 1) no valid entry in REST or 2) the oldest instr. is not ready
    if(oldest_index == -1){
#ifdef DEBUG_LOG
        printf("REST_in_order_get_oldest: oldest:%d, src1_ready:%d, src1_ready:%d\n", oldest_index, t->REST_Entries[oldest_index].inst.src1_ready, t->REST_Entries[oldest_index].inst.src2_ready);
#endif
        return false;
    }
    else if((t->REST_Entries[oldest_index].inst.src1_ready != 1) || (t->REST_Entries[oldest_index].inst.src2_ready != 1)){
#ifdef DEBUG_LOG
        printf("REST_in_order_get_oldest: oldest:%d, src1_ready:%d, src1_ready:%d\n", oldest_index, t->REST_Entries[oldest_index].inst.src1_ready, t->REST_Entries[oldest_index].inst.src2_ready);
#endif
        return false;
    }    
    else{
#ifdef DEBUG_LOG
        printf("REST_in_order_get_oldest(OK): oldest:%d, src1_tag:%d, src2_tag:%d\n", oldest_index, t->REST_Entries[oldest_index].inst.src1_tag, t->REST_Entries[oldest_index].inst.src2_tag);
#endif
        *SC_inst = t->REST_Entries[oldest_index].inst;
        return true;
    }
    

}


bool REST_ooo_get_oldest(REST *t, Inst_Info *SC_inst){
    
    int ii, oldest_index = -1;
    uint64_t oldest_inst = 0xFFFFFFFFFFFFFFFF;
    
    for(ii = 0; ii < NUM_REST_ENTRIES; ii++){
        
        if((t->REST_Entries[ii].valid == true) && (t->REST_Entries[ii].scheduled == false) && (t->REST_Entries[ii].inst.src1_ready == 1) && (t->REST_Entries[ii].inst.src2_ready == 1)){
            if(t->REST_Entries[ii].inst.inst_num < oldest_inst){
                oldest_inst = t->REST_Entries[ii].inst.inst_num;
                oldest_index = ii;
            }
        }
    }
    
    // break the SC_latch loop if 1) no valid entry in REST
    if(oldest_index == -1){
#ifdef DEBUG_LOG
        printf("REST_ooo_get_oldest: oldest:%d, src1_ready:%d, src1_ready:%d\n", oldest_index, t->REST_Entries[oldest_index].inst.src1_ready, t->REST_Entries[oldest_index].inst.src2_ready);
#endif
        return false;
    }
    else{
#ifdef DEBUG_LOG
        printf("REST_ooo_get_oldest(OK): oldest:%d, src1_tag:%d, src2_tag:%d\n", oldest_index, t->REST_Entries[oldest_index].inst.src1_tag, t->REST_Entries[oldest_index].inst.src2_tag);
#endif
        *SC_inst = t->REST_Entries[oldest_index].inst;
        return true;
    }
    
}

