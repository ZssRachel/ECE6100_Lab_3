#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "rob.h"


extern int32_t NUM_ROB_ENTRIES;

/////////////////////////////////////////////////////////////
// Init function initializes the ROB
/////////////////////////////////////////////////////////////

ROB* ROB_init(void){
  int ii;
  ROB *t = (ROB *) calloc (1, sizeof (ROB));
  for(ii=0; ii<MAX_ROB_ENTRIES; ii++){
    t->ROB_Entries[ii].valid=false;
    t->ROB_Entries[ii].ready=false;
  }
  t->head_ptr=0;
  t->tail_ptr=0;
  return t;
}

/////////////////////////////////////////////////////////////
// Print State
/////////////////////////////////////////////////////////////
void ROB_print_state(ROB *t){
 int ii = 0;
  printf("Printing ROB \n");
  printf("Entry  Inst   Valid   ready\n");
  for(ii = 0; ii < NUM_ROB_ENTRIES; ii++) {
    printf("%5d ::  %d\t", ii, (int)t->ROB_Entries[ii].inst.inst_num);
    printf(" %5d\t", t->ROB_Entries[ii].valid);
    printf(" %5d\n", t->ROB_Entries[ii].ready);
  }
  printf("\n");
}

/////////////////////////////////////////////////////////////
//------- DO NOT CHANGE THE CODE ABOVE THIS LINE -----------
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
// If there is space in ROB return true, else false
/////////////////////////////////////////////////////////////

bool ROB_check_space(ROB *t){

#ifdef DEBUG_LOG
    printf("ROB_check_space: head_ptr: %d, tail_ptr: %d\n", t->head_ptr, t->tail_ptr);
#endif
    
    if(t->ROB_Entries[t->tail_ptr].valid == true)
        return false;
    else
        return true;
    
#if 0
    if(t->tail_ptr == (t->head_ptr-1))
        return false;
    else if((t->tail_ptr == (NUM_ROB_ENTRIES-1)) && t->tail_ptr == 0)
        return false;
    else
        return true;
#endif
}

/////////////////////////////////////////////////////////////
// insert entry at tail, increment tail (do check_space first)
/////////////////////////////////////////////////////////////

int ROB_insert(ROB *t, Inst_Info inst){
  
    int prev_tail;
    
    prev_tail = t->tail_ptr;

    t->ROB_Entries[t->tail_ptr].inst = inst;
    t->ROB_Entries[t->tail_ptr].valid = true;
  
    if(t->tail_ptr < (NUM_ROB_ENTRIES-1))
        t->tail_ptr += 1;
    else
        t->tail_ptr = 0;

#ifdef DEBUG_LOG
    printf("ROB_insert: inst. %ld, tail_ptr: %d\n", t->ROB_Entries[prev_tail].inst.inst_num, prev_tail);
#endif
    
    return (prev_tail);
    

}

/////////////////////////////////////////////////////////////
// Once an instruction finishes execution, mark rob entry as done
/////////////////////////////////////////////////////////////

void ROB_mark_ready(ROB *t, Inst_Info inst){
    
    if(t->ROB_Entries[inst.dr_tag].valid)
    {
        t->ROB_Entries[inst.dr_tag].ready = true;
        t->ROB_Entries[inst.dr_tag].inst = inst;  //update Inst Info in ROB
#ifdef DEBUG_LOG
        printf("ROB_mark_ready: inst. %ld, ROB_ID:%d\n", t->ROB_Entries[inst.dr_tag].inst.inst_num, inst.dr_tag);
#endif
        
    }
    else
    {
#ifdef DEBUG_LOG
        printf("ROB_mark_ready: fail! inst. %ld, ROB_ID:%d\n", t->ROB_Entries[inst.dr_tag].inst.inst_num, inst.dr_tag);
#endif
        assert(1);
    }
}

/////////////////////////////////////////////////////////////
// Find whether the prf-rob entry is ready
/////////////////////////////////////////////////////////////

bool ROB_check_ready(ROB *t, int tag){
    
    int ii = 0;
    
    for(ii=0; ii<NUM_ROB_ENTRIES; ii++){
        if((t->ROB_Entries[ii].inst.dr_tag == tag) && (t->ROB_Entries[ii].ready == true) && (t->ROB_Entries[ii].valid == true))
            return true;
    }
    
    return false;
    
}


/////////////////////////////////////////////////////////////
// Check if the oldest ROB entry is ready for commit
/////////////////////////////////////////////////////////////

bool ROB_check_head(ROB *t){
    
    if((t->ROB_Entries[t->head_ptr].ready == true) && (t->ROB_Entries[t->head_ptr].valid == true))
        return true;
    else
        return false;
}

/////////////////////////////////////////////////////////////
// Remove oldest entry from ROB (after ROB_check_head)
/////////////////////////////////////////////////////////////

Inst_Info ROB_remove_head(ROB *t){
    
    int prev_head;
    
    prev_head = t->head_ptr;
    
    if(t->head_ptr < NUM_ROB_ENTRIES-1)
        t->head_ptr = t->head_ptr + 1;
    else
        t->head_ptr = 0;
    
    t->ROB_Entries[prev_head].valid = false;
    t->ROB_Entries[prev_head].ready = false;

#ifdef DEBUG_LOG
    printf("ROB_remove_head: inst. %ld, ROB_ID:%d\n", t->ROB_Entries[prev_head].inst.inst_num, prev_head);
#endif
    
    return t->ROB_Entries[prev_head].inst;
    

}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
