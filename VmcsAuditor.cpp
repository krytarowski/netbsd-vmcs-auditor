// VmcsAuditor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

using namespace std;

void PrintAsciiArt() {
	printf("\n\n");
	printf(" __      __                                   _ _ _             \n");
	printf(" \\ \\    / /                    /\\            | (_) |            \n");
	printf("  \\ \\  / / __ ___   ___ ___   /  \\  _   _  __| |_| |_ ___  _ __ \n");
	printf("   \\ \\/ / '_ ` _ \\ / __/ __| / /\\ \\| | | |/ _` | | __/ _ \\| '__|\n");
	printf("    \\  /| | | | | | (__\\__ \\/ ____ \\ |_| | (_| | | || (_) | |   \n");
	printf("     \\/ |_| |_| |_|\\___|___/_/    \\_\\__,_|\\__,_|_|\\__\\___/|_|   \n\n");

}


uint64_t ReadInput(const char* Message, int64_t DefaultValue) {

	std::cout << Message << " [ Default : 0x"<< DefaultValue << " ]"<<endl;

	string numberstr = std::to_string(DefaultValue);
	int64_t number = DefaultValue;
	std::string input;
	std::getline(std::cin, input);
	if (!input.empty()) {
		std::istringstream stream(input);

		stream >> numberstr;
	}

	std::stringstream ss;
	ss << std::hex << numberstr;
	ss >> number;

	cout << endl << "  =======>  " << Message << " Set to : " << std::hex  << number << endl;


	return number;

}



int main()
{
	
	PrintAsciiArt();
	printf("[*] This project tends to ease the process of checking VMCS state checking as long as Intel doesn't give a precise error cause for each field.\n");
	printf("[*] It's based on Bochs checks on VMCS but can be used independently.\n");
	printf("\n______________________	 Note	 _________________________\n\n");
	printf("[*] Note : You should enter all the values in HEX format.\n");


	printf("\n______________________ CAPABILITIES BITS _________________________\n\n");
	// Get MSRs
	printf("[*] Let's first get MSRs to see what your target machine supports.\n");
	printf("[*] You can use your driver with _readmsr or simply use windbg's \"RDMSR\" command in order to get these values.\n");
	printf("[*] e.g rdmsr 0x481 in windbg gives the result that you need to insert here.\n\n");


	//  MSR_IA32_VMX_PINBASED_CTLS          0x481
	uint32_t _vmx_pin_vmexec_ctrl_supported_bits = ReadInput("MSR_IA32_VMX_PINBASED_CTLS (0x481)", 0x0);




	//  MSR_IA32_VMX_PROCBASED_CTLS         0x482
	uint32_t _vmx_proc_vmexec_ctrl_supported_bits = ReadInput("MSR_IA32_VMX_PROCBASED_CTLS (0x482)", 0x0);


	// MSR_IA32_VMX_PROCBASED_CTLS2        0x48B
	uint32_t _vmx_vmexec_ctrl2_supported_bits =  ReadInput("MSR_IA32_VMX_PROCBASED_CTLS2 (0x48B)", 0x0);



	// MSR_IA32_VMX_EXIT_CTLS              0x483
	uint32_t _vmx_vmexit_ctrl_supported_bits = ReadInput("MSR_IA32_VMX_EXIT_CTLS (0x483)", 0x0);

	// MSR_IA32_VMX_ENTRY_CTLS             0x484
	uint32_t _vmx_vmentry_ctrl_supported_bits = ReadInput("MSR_IA32_VMX_ENTRY_CTLS (0x484)", 0x0);

	// MSR_IA32_VMX_EPT_VPID_CAP           0x48C
	uint64_t _vmx_ept_vpid_cap_supported_bits = ReadInput("MSR_IA32_VMX_EPT_VPID_CAP (0x48C)", 0x0);

	//  MSR_IA32_VMX_VMFUNC                 0x491
	uint64_t _vmx_vmfunc_supported_bits = ReadInput("MSR_IA32_VMX_VMFUNC (0x491)", 0x0);

	//  MSR_IA32_VMX_CR0_FIXED0             0x486
	uint32_t _cr0_suppmask_0 = ReadInput("MSR_IA32_VMX_CR0_FIXED0 (0x486)", 0x0);

	//  MSR_IA32_VMX_CR0_FIXED1             0x487
	uint32_t _cr0_suppmask_1 = ReadInput("MSR_IA32_VMX_CR0_FIXED1 (0x487)", 0x0);

	//  MSR_IA32_VMX_CR4_FIXED0             0x488
	uint32_t _cr4_suppmask_0 = ReadInput("MSR_IA32_VMX_CR4_FIXED0 (0x488)", 0x0);

	//  MSR_IA32_VMX_CR4_FIXED1             0x489
	uint32_t _cr4_suppmask_1 = ReadInput("MSR_IA32_VMX_CR4_FIXED1 (0x489)", 0x0);



	/// Preparing VMCS Layout
	
	VMCS_CACHE *vm = (VMCS_CACHE*) malloc(sizeof(VMCS_CACHE));

	printf("______________________ HOST STATE _________________________\n");

	// Get host states
	vm->host_state.cr0 = ReadInput("host_state.cr0 ", 0x0);
	vm->host_state.cr3 = ReadInput("host_state.cr3 ", 0x0);
	vm->host_state.cr4 = ReadInput("host_state.cr4 ", 0x0);
	vm->host_state.efer_msr = ReadInput("host_state.efer_msr ", 0x0);
	vm->host_state.fs_base = ReadInput("host_state.fs_base ", 0x0);
	vm->host_state.gdtr_base = ReadInput("host_state.gdtr_base ", 0x0);
	vm->host_state.gs_base = ReadInput("host_state.gs_base ", 0x0);
	vm->host_state.idtr_base = ReadInput("host_state.idtr_base ", 0x0);
	vm->host_state.pat_msr = ReadInput("host_state.pat_msr ", 0x0);
	vm->host_state.rip = ReadInput("host_state.rip ", 0x0);
	vm->host_state.rsp = ReadInput("host_state.rsp ", 0x0);
	vm->host_state.segreg_selector[0] = ReadInput("host_state.segreg_selector_es ", 0x0);
	vm->host_state.segreg_selector[1] = ReadInput("host_state.segreg_selector_cs ", 0x0);
	vm->host_state.segreg_selector[2] = ReadInput("host_state.segreg_selector_ss ", 0x0);
	vm->host_state.segreg_selector[3] = ReadInput("host_state.segreg_selector_ds ", 0x0);
	vm->host_state.segreg_selector[4] = ReadInput("host_state.segreg_selector_fs ", 0x0);
	vm->host_state.segreg_selector[5] = ReadInput("host_state.segreg_selector_gs ", 0x0);
	vm->host_state.sysenter_cs_msr = ReadInput("host_state.sysenter_cs_msr ", 0x0);
	vm->host_state.sysenter_eip_msr = ReadInput("host_state.sysenter_eip_msr ", 0x0);
	vm->host_state.sysenter_esp_msr = ReadInput("host_state.sysenter_esp_msr ", 0x0);
	vm->host_state.tr_base = ReadInput("host_state.tr_base ", 0x0);




	printf("\n______________________ GENERAL DETAILS _________________________\n");
	bool IsVMResume = (bool)  ReadInput("Is it VM Resume (0x1) ? otherwise it's VM Launch (0x0) ", 0x0);
	uint64_t RevisionID = ReadInput("What's the RevisionID ? ", 0x4);
	uint64_t VMXON_Pointer = ReadInput("What's the VMXON Pointer ? ", 0x0);

	printf("\n______________________ GUEST & CTRL BITS _________________________\n");


	// Call Checker 
	CheckVMXState(vm,IsVMResume, VMXON_Pointer, RevisionID,
		_vmx_pin_vmexec_ctrl_supported_bits, _vmx_proc_vmexec_ctrl_supported_bits,
		_vmx_vmexec_ctrl2_supported_bits, _vmx_vmexit_ctrl_supported_bits,
		_vmx_vmentry_ctrl_supported_bits, _vmx_ept_vpid_cap_supported_bits,
		_vmx_vmfunc_supported_bits, _cr0_suppmask_0, _cr0_suppmask_1,
		_cr4_suppmask_0, _cr4_suppmask_1);

	printf("\n\n\n\n\n______________________ That's it ;) _________________________\n");

	return EXIT_SUCCESS;
}
