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
  for(ii = 0; ii < 7; ii++) {
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
    
    if(t->tail_ptr == (t->head_ptr-1))
        return false;
    else
        return true;
}

/////////////////////////////////////////////////////////////
// insert entry at tail, increment tail (do check_space first)
/////////////////////////////////////////////////////////////

int ROB_insert(ROB *t, Inst_Info inst){
  
    if(ROB_check_space(t) == true)
    {
        if(t->tail_ptr < (MAX_ROB_ENTRIES-1))
          t->tail_ptr += 1;
        else
          t->tail_ptr = 0;
        
        //memcpy(t->ROB_Entries[t->tail_ptr].inst, inst, sizeof(inst));
        t->ROB_Entries[t->tail_ptr].inst = inst;
        t->ROB_Entries[t->tail_ptr].valid = true;
    }

}

/////////////////////////////////////////////////////////////
// Once an instruction finishes execution, mark rob entry as done
/////////////////////////////////////////////////////////////

void ROB_mark_ready(ROB *t, Inst_Info inst){
    
    t->ROB_Entries[inst.dr_tag].ready = true;
    

}

/////////////////////////////////////////////////////////////
// Find whether the prf-rob entry is ready
/////////////////////////////////////////////////////////////

bool ROB_check_ready(ROB *t, int tag){
    
    int ii = 0;
    
    for(ii=0; ii<MAX_ROB_ENTRIES; ii++){
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
    
    if(t->head_ptr < MAX_ROB_ENTRIES-1)
        t->head_ptr = t->head_ptr + 1;
    else
        t->head_ptr = 0;

}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
