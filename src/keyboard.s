# Copyright (C) 2018, Jaybill McCarthy
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
 
# Adapted from examples written by Martynas Kunigelis in 1996
 
		.file	"keyboard.s"
 #
 # externals (optional):
 #
		.extern ___djgpp_base_address
		.extern ___djgpp_ds_alias
 		.extern	___djgpp_dos_sel
 #
 # public functions and variables:
 #
		.global _keyboard_map
		.global _keyboard_init
		.global _keyboard_close
		.global _keyboard_chain

		.text
		
		.align 	4

locking_region_start:

_keyboard_map:	.space 	0x80, 0

old_vector:
old_vector_ofs:	.long	0
old_vector_sel:	.word	0
chain_flag:	.long 	1


		.align	4

handler_procedure:

 #
 # .. will be called every time a key is pressed/released
 #                    
		pushl	%eax
		pushl	%edx
		pushw	%ds
 #
 # Load DS with our data selector
 #
 		movw	%cs:___djgpp_ds_alias, %ds
 #
 # Read the scancode from keyboard port and update keyboard_map
 #
		inb	$0x60, %al
		movb	%al, %dl
		andl	$0x7f, %edx
		testb	$0x80, %al
		setz	_keyboard_map(%edx)
		
 #
 # Chain if flag is set, otherwise do what's necessary and return
 #
		cmpl	$0, chain_flag
		jne	handler_chain
 #
 # Acknowledge keyboard and interrupt contollers
 #
		inb	$0x61, %al
		orb	$0x80, %al
		outb	%al, $0x61
		andb	$0x7f, %al
		outb	%al, $0x61
		movb	$0x20, %al
		outb	%al, $0x20

		popw	%ds
		popl	%edx
		popl	%eax
		sti
		iret

		.align	4

handler_chain:  popw	%ds
		popl	%edx
		popl	%eax
		ljmp	*	%cs:(old_vector)

locking_region_end:


		.align	4
_keyboard_init:

 #
 # int keyboard_init(void);
 #
 # Initializes the keyboard handler and hooks the keyboard interrupt.
 # Returns -1 on failure, zero on success
 #
		pushl	%esi
		pushl	%edi
		pushl	%ebx
 #
 # First, we need to lock the handler and memory it touches, so
 # it doesn't get swapped out to disk.
 #
		leal	locking_region_start, %ecx
		leal	locking_region_end, %edi
		subl	%ecx, %edi
		addl	___djgpp_base_address, %ecx
		shldl	$16, %ecx, %ebx		# ecx -> bx:cx
		shldl	$16, %edi, %esi         # edi -> si:di
		movw    $0x0600, %ax		# lock linear region
		int	$0x31
		jc	init_error
 #
 # Now we need to save the old interrupt vector, so we can restore
 # it later and also to know where to jump if chaining.
 #
		movw	$0x0204, %ax    	# get pm int vector
		movb	$0x09, %bl
		int	$0x31
		movw	%cx, old_vector_sel
		movl	%edx, old_vector_ofs
 #
 # Make sure we chain after initialization.
 #
		movl	$1, chain_flag
 #
 # Set the interrupt vector to point to our handler.
 #
		movw	%cs, %cx    		
		leal	handler_procedure, %edx
		movb	$0x09, %bl
		movw	$0x0205, %ax    	# set pm int vector
		int	$0x31
 #*
 #* Actually we would have to unlock the locked region on failure
 #* here. But since most programs would exit with an error message
 #* in such case, there's no need to worry.
 #*

init_error:

 #
 # This sets EAX to -1 if CF is set and to 0 atherwise
 #
		movl	$0, %eax
		sbbl	$0, %eax
		
		popl	%ebx
		popl	%edi
		popl	%esi
		ret


		.align 4
_keyboard_close:

 #
 # void keyboard_close(void);
 #
 # Shuts the keyboard handler down.
 #
		pushl	%esi
		pushl	%edi
		pushl	%ebx
 #
 # Unlock the region we locked at initialization
 # 
		leal	locking_region_start, %ecx
		leal	locking_region_end, %edi
		subl	%ecx, %edi
		addl	___djgpp_base_address, %ecx
		shldl	$16, %ecx, %ebx
		shldl	$16, %edi, %esi
		movw	$0x0601, %ax  		# unlock linear region
		int	$0x31                   
 #
 # Restore the interrupt vector to its previous value
 #
		movw	old_vector_sel, %cx
		movl	old_vector_ofs, %edx
		movb	$0x09, %bl
		movw	$0x0205, %ax            # set pm int vector
		int	$0x31

		popl	%ebx
		popl	%edi
		popl	%esi
		ret
 #
 # void keyboard_chain(int toggle);
 #
		.align	4
_keyboard_chain:
		cmpl	$0, 4(%esp)
		je	chain_off
chain_on:

 #
 # Set the chain_flag and clear BIOS shift/ctrl/alt status bits:
 #
		movl	$1, chain_flag

		push	%es
		movw	___djgpp_dos_sel, %es
		andb	$0xf0, %es:0x417
		pop	%es
		jmp	chain_done
chain_off:
		movl	$0, chain_flag
chain_done:     ret
