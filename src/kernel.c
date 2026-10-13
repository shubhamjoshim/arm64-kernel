#include "gic.h"
#include "uart.h"
#include "kprintf.h"
#include "timer.h"
#include <stdint.h>
#define ESR_EC_SHIFT 26U
#define MASK 0x3FU
#define EC_BRK_AARCH64 0x3CU
#define EC_INSTR_ABORT_SAME_EL 0x21U
#define EC_DATA_ABORT_SAME_EL 0x25U

uint64_t get_current_el(void);


struct exception_frame {
    uint64_t x0;
    uint64_t x1;
    uint64_t x2;
    uint64_t x3;
    uint64_t x4;
    uint64_t x5;
    uint64_t x6;
    uint64_t x7;
    uint64_t x8;
    uint64_t x9;
    uint64_t x10;
    uint64_t x11;
    uint64_t x12;
    uint64_t x13;
    uint64_t x14;
    uint64_t x15;
    uint64_t x16;
    uint64_t x17;
    uint64_t x18;
    uint64_t x19;
    uint64_t x20;
    uint64_t x21;
    uint64_t x22;
    uint64_t x23;
    uint64_t x24;
    uint64_t x25;
    uint64_t x26;
    uint64_t x27;
    uint64_t x28;
    uint64_t x29;
    uint64_t x30;

    uint64_t elr;
    uint64_t spsr;
    uint64_t esr;
};


void kernel_panic(const char *reason, struct exception_frame *frame){
    kprintf("\n========== KERNEL PANIC ==========\n");
    kprintf("Reason: %s\n", reason);
    kprintf("ELR_EL1:  0x%x\n", frame->elr);
    kprintf("ESR_EL1:  0x%x\n", frame->esr);
    kprintf("SPSR_EL1: 0x%x\n", frame->spsr);
    kprintf("==================================\n");
    while (1) {

    }

}
static uint64_t read_far_el1(void){
  uint64_t far;

  __asm__ volatile("mrs %0, FAR_EL1": "=r"(far));

  return far;
}
void irq_handler(struct exception_frame *frame){
  (void) frame;

  uint32_t iar = gic_acknowledge_irq();
  uint32_t id = gic_get_irq_id(iar);

  if(id == 30U){
    timer_disable();
    kprintf("TIMER IRQ\n");
  }
  gic_end_irq(iar);
}
void exception_handler(struct exception_frame *frame){ // elr tells us where the exception happened; esr tells us why it happened, both values are hex
                                                    
  uint64_t elr = frame -> elr;
  uint64_t esr = frame -> esr;
  kprintf("EXCEPTION: \nELR_E1: 0x%x\n",elr);
  kprintf("ESR_E1: 0x%x\n",esr);

  uint64_t ec = (esr >> ESR_EC_SHIFT) & (MASK); //the type of exception lives in the [31:26] bits so we right shift by 26 so that the first 6 bits are these ones. Masking with 0x3F 
                                     //helps us to get to know exactly which bit of the six were 1, i.e which type of exception
  kprintf("Exception class: 0x%x\n",ec);
  
  if(ec == EC_BRK_AARCH64){
    uart_put_string("TYPE: BRK\n");
    uint64_t comment = (esr) & (0xFFFF); //value of BRK is stored in first sixteen bits of esr
    kprintf("BRK value: %d\n",(int)comment);
    frame->elr += 4;
  }
  else if(ec == EC_DATA_ABORT_SAME_EL){ // anyother unhandled exception
    uint64_t far = read_far_el1();

    kprintf("Data Abort\n");
    kprintf("Fault Address: 0x%x\n",far);

    kernel_panic("Data Abort", frame);
  }
  else if(ec == EC_INSTR_ABORT_SAME_EL){
    uint64_t far = read_far_el1();

    kprintf("Instruction Abort\n");
    kprintf("Fault Address: 0x%x\n",far);
    kernel_panic("Instruction Abort", frame);
  }
  else{
    kernel_panic("Unhandled exception", frame);
  }

}

void kernel_main(void)
{
    uart_put_string("UART console ready\n");

    uint64_t el=get_current_el(); //gets the current EL level from boot.S
    kprintf("Current EL: %d\n",(int)el);

    uart_put_string("Before Exception\n");
    __asm__ volatile ("brk #0"); //intentionally invoking a syn spx error
    uart_put_string("After Exception\n"); //should print this if exception handling is successful
    uint64_t freq=timer_get_frequency();
    kprintf("Timer frequency: %d\n",(int) freq);
    gic_enable_timer_irq();

    timer_set_ticks(freq);
    timer_enable();
    kprintf("Waiting for irq\n");
    __asm__ volatile("msr DAIFClr, #2");
    while (1) {
    }
}
