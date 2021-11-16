﻿param([String]$debugfile = "");

# This powershell file has been generated by the IAR Embedded Workbench
# C - SPY Debugger, as an aid to preparing a command line for running
# the cspybat command line utility using the appropriate settings.
#
# Note that this file is generated every time a new debug session
# is initialized, so you may want to move or rename the file before
# making changes.
#
# You can launch cspybat by typing Powershell.exe -File followed by the name of this batch file, followed
# by the name of the debug file (usually an ELF / DWARF or UBROF file).
#
# Read about available command line parameters in the C - SPY Debugging
# Guide. Hints about additional command line parameters that may be
# useful in specific cases :
#   --download_only   Downloads a code image without starting a debug
#                     session afterwards.
#   --silent          Omits the sign - on message.
#   --timeout         Limits the maximum allowed execution time.
#


if ($debugfile -eq "")
{
& "F:\IAR_for_stm8\common\bin\cspybat" -f "F:\C++\stm8_measure\stm8l051\STM8L15X-05X-16X-en.stsw-stm8016\STM8L15x-16x-05x-AL31-L_StdPeriph_Lib\Project\STM8L15x_StdPeriph_Template\EWSTM8\settings\project.STM8L15X_MD(STM8L15xx4_6).general.xcl" --backend -f "F:\C++\stm8_measure\stm8l051\STM8L15X-05X-16X-en.stsw-stm8016\STM8L15x-16x-05x-AL31-L_StdPeriph_Lib\Project\STM8L15x_StdPeriph_Template\EWSTM8\settings\project.STM8L15X_MD(STM8L15xx4_6).driver.xcl" 
}
else
{
& "F:\IAR_for_stm8\common\bin\cspybat" -f "F:\C++\stm8_measure\stm8l051\STM8L15X-05X-16X-en.stsw-stm8016\STM8L15x-16x-05x-AL31-L_StdPeriph_Lib\Project\STM8L15x_StdPeriph_Template\EWSTM8\settings\project.STM8L15X_MD(STM8L15xx4_6).general.xcl" --debug_file=$debugfile --backend -f "F:\C++\stm8_measure\stm8l051\STM8L15X-05X-16X-en.stsw-stm8016\STM8L15x-16x-05x-AL31-L_StdPeriph_Lib\Project\STM8L15x_StdPeriph_Template\EWSTM8\settings\project.STM8L15X_MD(STM8L15xx4_6).driver.xcl" 
}
