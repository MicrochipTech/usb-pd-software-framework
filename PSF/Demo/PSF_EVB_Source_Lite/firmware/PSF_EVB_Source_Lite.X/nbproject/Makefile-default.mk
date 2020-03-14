#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PSF_EVB_Source_Lite.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PSF_EVB_Source_Lite.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/config/default/peripheral/clock/plib_clock.c ../src/config/default/peripheral/eic/plib_eic.c ../src/config/default/peripheral/nvic/plib_nvic.c ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/default/peripheral/port/plib_port.c ../src/config/default/peripheral/sercom/spim/plib_sercom0_spi.c ../src/config/default/peripheral/sercom/usart/plib_sercom3_usart.c ../src/config/default/peripheral/tc/plib_tc0.c ../src/config/default/stdio/xc32_monitor.c ../src/config/default/initialization.c ../src/config/default/interrupts.c ../src/config/default/exceptions.c ../src/config/default/startup_xc32.c ../src/config/default/libc_syscalls.c ../../PowerDelivery/PSFSourceLite_App.c ../../PowerDelivery/PSFSourceLite_BootCfg.c ../../../../SOC_Portable/SAMD20/Drivers.c ../../../../Source/PB/pb_helper.c ../../../../Source/PB/pb_mngr.c ../../../../Source/PD3_0/debug.c ../../../../Source/PD3_0/int_globals.c ../../../../Source/PD3_0/pd_main.c ../../../../Source/PD3_0/pd_timer.c ../../../../Source/PD3_0/policy_engine.c ../../../../Source/PD3_0/policy_engine_fwup.c ../../../../Source/PD3_0/policy_engine_snk.c ../../../../Source/PD3_0/policy_engine_src.c ../../../../Source/PD3_0/policy_manager.c ../../../../Source/PD3_0/portpower_control.c ../../../../Source/PD3_0/protocol_layer.c ../../../../Source/PD3_0/typeC_control.c ../../../../Source/PD3_0/upd_hw.c ../../../../Source/PD3_0/upd_interrupts.c ../src/main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1984496892/plib_clock.o ${OBJECTDIR}/_ext/60167341/plib_eic.o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ${OBJECTDIR}/_ext/1865521619/plib_port.o ${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o ${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o ${OBJECTDIR}/_ext/829342655/plib_tc0.o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ${OBJECTDIR}/_ext/1171490990/initialization.o ${OBJECTDIR}/_ext/1171490990/interrupts.o ${OBJECTDIR}/_ext/1171490990/exceptions.o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o ${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o ${OBJECTDIR}/_ext/1671623073/Drivers.o ${OBJECTDIR}/_ext/1936174342/pb_helper.o ${OBJECTDIR}/_ext/1936174342/pb_mngr.o ${OBJECTDIR}/_ext/840851172/debug.o ${OBJECTDIR}/_ext/840851172/int_globals.o ${OBJECTDIR}/_ext/840851172/pd_main.o ${OBJECTDIR}/_ext/840851172/pd_timer.o ${OBJECTDIR}/_ext/840851172/policy_engine.o ${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o ${OBJECTDIR}/_ext/840851172/policy_engine_snk.o ${OBJECTDIR}/_ext/840851172/policy_engine_src.o ${OBJECTDIR}/_ext/840851172/policy_manager.o ${OBJECTDIR}/_ext/840851172/portpower_control.o ${OBJECTDIR}/_ext/840851172/protocol_layer.o ${OBJECTDIR}/_ext/840851172/typeC_control.o ${OBJECTDIR}/_ext/840851172/upd_hw.o ${OBJECTDIR}/_ext/840851172/upd_interrupts.o ${OBJECTDIR}/_ext/1360937237/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1984496892/plib_clock.o.d ${OBJECTDIR}/_ext/60167341/plib_eic.o.d ${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d ${OBJECTDIR}/_ext/1865521619/plib_port.o.d ${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o.d ${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o.d ${OBJECTDIR}/_ext/829342655/plib_tc0.o.d ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d ${OBJECTDIR}/_ext/1171490990/initialization.o.d ${OBJECTDIR}/_ext/1171490990/interrupts.o.d ${OBJECTDIR}/_ext/1171490990/exceptions.o.d ${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d ${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o.d ${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o.d ${OBJECTDIR}/_ext/1671623073/Drivers.o.d ${OBJECTDIR}/_ext/1936174342/pb_helper.o.d ${OBJECTDIR}/_ext/1936174342/pb_mngr.o.d ${OBJECTDIR}/_ext/840851172/debug.o.d ${OBJECTDIR}/_ext/840851172/int_globals.o.d ${OBJECTDIR}/_ext/840851172/pd_main.o.d ${OBJECTDIR}/_ext/840851172/pd_timer.o.d ${OBJECTDIR}/_ext/840851172/policy_engine.o.d ${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o.d ${OBJECTDIR}/_ext/840851172/policy_engine_snk.o.d ${OBJECTDIR}/_ext/840851172/policy_engine_src.o.d ${OBJECTDIR}/_ext/840851172/policy_manager.o.d ${OBJECTDIR}/_ext/840851172/portpower_control.o.d ${OBJECTDIR}/_ext/840851172/protocol_layer.o.d ${OBJECTDIR}/_ext/840851172/typeC_control.o.d ${OBJECTDIR}/_ext/840851172/upd_hw.o.d ${OBJECTDIR}/_ext/840851172/upd_interrupts.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1984496892/plib_clock.o ${OBJECTDIR}/_ext/60167341/plib_eic.o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ${OBJECTDIR}/_ext/1865521619/plib_port.o ${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o ${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o ${OBJECTDIR}/_ext/829342655/plib_tc0.o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ${OBJECTDIR}/_ext/1171490990/initialization.o ${OBJECTDIR}/_ext/1171490990/interrupts.o ${OBJECTDIR}/_ext/1171490990/exceptions.o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o ${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o ${OBJECTDIR}/_ext/1671623073/Drivers.o ${OBJECTDIR}/_ext/1936174342/pb_helper.o ${OBJECTDIR}/_ext/1936174342/pb_mngr.o ${OBJECTDIR}/_ext/840851172/debug.o ${OBJECTDIR}/_ext/840851172/int_globals.o ${OBJECTDIR}/_ext/840851172/pd_main.o ${OBJECTDIR}/_ext/840851172/pd_timer.o ${OBJECTDIR}/_ext/840851172/policy_engine.o ${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o ${OBJECTDIR}/_ext/840851172/policy_engine_snk.o ${OBJECTDIR}/_ext/840851172/policy_engine_src.o ${OBJECTDIR}/_ext/840851172/policy_manager.o ${OBJECTDIR}/_ext/840851172/portpower_control.o ${OBJECTDIR}/_ext/840851172/protocol_layer.o ${OBJECTDIR}/_ext/840851172/typeC_control.o ${OBJECTDIR}/_ext/840851172/upd_hw.o ${OBJECTDIR}/_ext/840851172/upd_interrupts.o ${OBJECTDIR}/_ext/1360937237/main.o

# Source Files
SOURCEFILES=../src/config/default/peripheral/clock/plib_clock.c ../src/config/default/peripheral/eic/plib_eic.c ../src/config/default/peripheral/nvic/plib_nvic.c ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/default/peripheral/port/plib_port.c ../src/config/default/peripheral/sercom/spim/plib_sercom0_spi.c ../src/config/default/peripheral/sercom/usart/plib_sercom3_usart.c ../src/config/default/peripheral/tc/plib_tc0.c ../src/config/default/stdio/xc32_monitor.c ../src/config/default/initialization.c ../src/config/default/interrupts.c ../src/config/default/exceptions.c ../src/config/default/startup_xc32.c ../src/config/default/libc_syscalls.c ../../PowerDelivery/PSFSourceLite_App.c ../../PowerDelivery/PSFSourceLite_BootCfg.c ../../../../SOC_Portable/SAMD20/Drivers.c ../../../../Source/PB/pb_helper.c ../../../../Source/PB/pb_mngr.c ../../../../Source/PD3_0/debug.c ../../../../Source/PD3_0/int_globals.c ../../../../Source/PD3_0/pd_main.c ../../../../Source/PD3_0/pd_timer.c ../../../../Source/PD3_0/policy_engine.c ../../../../Source/PD3_0/policy_engine_fwup.c ../../../../Source/PD3_0/policy_engine_snk.c ../../../../Source/PD3_0/policy_engine_src.c ../../../../Source/PD3_0/policy_manager.c ../../../../Source/PD3_0/portpower_control.c ../../../../Source/PD3_0/protocol_layer.c ../../../../Source/PD3_0/typeC_control.c ../../../../Source/PD3_0/upd_hw.c ../../../../Source/PD3_0/upd_interrupts.c ../src/main.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PSF_EVB_Source_Lite.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATSAMD20E16
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1984496892/plib_clock.o: ../src/config/default/peripheral/clock/plib_clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1984496892" 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1984496892/plib_clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1984496892/plib_clock.o.d" -o ${OBJECTDIR}/_ext/1984496892/plib_clock.o ../src/config/default/peripheral/clock/plib_clock.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/60167341/plib_eic.o: ../src/config/default/peripheral/eic/plib_eic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/60167341" 
	@${RM} ${OBJECTDIR}/_ext/60167341/plib_eic.o.d 
	@${RM} ${OBJECTDIR}/_ext/60167341/plib_eic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/60167341/plib_eic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/60167341/plib_eic.o.d" -o ${OBJECTDIR}/_ext/60167341/plib_eic.o ../src/config/default/peripheral/eic/plib_eic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1865468468/plib_nvic.o: ../src/config/default/peripheral/nvic/plib_nvic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1865468468" 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ../src/config/default/peripheral/nvic/plib_nvic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o: ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1593096446" 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1865521619/plib_port.o: ../src/config/default/peripheral/port/plib_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1865521619" 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1865521619/plib_port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1865521619/plib_port.o.d" -o ${OBJECTDIR}/_ext/1865521619/plib_port.o ../src/config/default/peripheral/port/plib_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o: ../src/config/default/peripheral/sercom/spim/plib_sercom0_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1955867357" 
	@${RM} ${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o.d" -o ${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o ../src/config/default/peripheral/sercom/spim/plib_sercom0_spi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o: ../src/config/default/peripheral/sercom/usart/plib_sercom3_usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/504274921" 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o.d" -o ${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o ../src/config/default/peripheral/sercom/usart/plib_sercom3_usart.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/829342655/plib_tc0.o: ../src/config/default/peripheral/tc/plib_tc0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/829342655" 
	@${RM} ${OBJECTDIR}/_ext/829342655/plib_tc0.o.d 
	@${RM} ${OBJECTDIR}/_ext/829342655/plib_tc0.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/829342655/plib_tc0.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/829342655/plib_tc0.o.d" -o ${OBJECTDIR}/_ext/829342655/plib_tc0.o ../src/config/default/peripheral/tc/plib_tc0.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/163028504/xc32_monitor.o: ../src/config/default/stdio/xc32_monitor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/163028504" 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ../src/config/default/stdio/xc32_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1171490990/initialization.o: ../src/config/default/initialization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1171490990/initialization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1171490990/initialization.o.d" -o ${OBJECTDIR}/_ext/1171490990/initialization.o ../src/config/default/initialization.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1171490990/interrupts.o: ../src/config/default/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1171490990/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1171490990/interrupts.o.d" -o ${OBJECTDIR}/_ext/1171490990/interrupts.o ../src/config/default/interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1171490990/exceptions.o: ../src/config/default/exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1171490990/exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1171490990/exceptions.o.d" -o ${OBJECTDIR}/_ext/1171490990/exceptions.o ../src/config/default/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1171490990/startup_xc32.o: ../src/config/default/startup_xc32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ../src/config/default/startup_xc32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1171490990/libc_syscalls.o: ../src/config/default/libc_syscalls.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ../src/config/default/libc_syscalls.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o: ../../PowerDelivery/PSFSourceLite_App.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/318156199" 
	@${RM} ${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o.d 
	@${RM} ${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o.d" -o ${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o ../../PowerDelivery/PSFSourceLite_App.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o: ../../PowerDelivery/PSFSourceLite_BootCfg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/318156199" 
	@${RM} ${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o.d 
	@${RM} ${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o.d" -o ${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o ../../PowerDelivery/PSFSourceLite_BootCfg.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1671623073/Drivers.o: ../../../../SOC_Portable/SAMD20/Drivers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1671623073" 
	@${RM} ${OBJECTDIR}/_ext/1671623073/Drivers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1671623073/Drivers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1671623073/Drivers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1671623073/Drivers.o.d" -o ${OBJECTDIR}/_ext/1671623073/Drivers.o ../../../../SOC_Portable/SAMD20/Drivers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1936174342/pb_helper.o: ../../../../Source/PB/pb_helper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1936174342" 
	@${RM} ${OBJECTDIR}/_ext/1936174342/pb_helper.o.d 
	@${RM} ${OBJECTDIR}/_ext/1936174342/pb_helper.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1936174342/pb_helper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1936174342/pb_helper.o.d" -o ${OBJECTDIR}/_ext/1936174342/pb_helper.o ../../../../Source/PB/pb_helper.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1936174342/pb_mngr.o: ../../../../Source/PB/pb_mngr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1936174342" 
	@${RM} ${OBJECTDIR}/_ext/1936174342/pb_mngr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1936174342/pb_mngr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1936174342/pb_mngr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1936174342/pb_mngr.o.d" -o ${OBJECTDIR}/_ext/1936174342/pb_mngr.o ../../../../Source/PB/pb_mngr.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/debug.o: ../../../../Source/PD3_0/debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/debug.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/debug.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/debug.o.d" -o ${OBJECTDIR}/_ext/840851172/debug.o ../../../../Source/PD3_0/debug.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/int_globals.o: ../../../../Source/PD3_0/int_globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/int_globals.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/int_globals.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/int_globals.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/int_globals.o.d" -o ${OBJECTDIR}/_ext/840851172/int_globals.o ../../../../Source/PD3_0/int_globals.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/pd_main.o: ../../../../Source/PD3_0/pd_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/pd_main.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/pd_main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/pd_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/pd_main.o.d" -o ${OBJECTDIR}/_ext/840851172/pd_main.o ../../../../Source/PD3_0/pd_main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/pd_timer.o: ../../../../Source/PD3_0/pd_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/pd_timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/pd_timer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/pd_timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/pd_timer.o.d" -o ${OBJECTDIR}/_ext/840851172/pd_timer.o ../../../../Source/PD3_0/pd_timer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/policy_engine.o: ../../../../Source/PD3_0/policy_engine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/policy_engine.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/policy_engine.o.d" -o ${OBJECTDIR}/_ext/840851172/policy_engine.o ../../../../Source/PD3_0/policy_engine.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o: ../../../../Source/PD3_0/policy_engine_fwup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o.d" -o ${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o ../../../../Source/PD3_0/policy_engine_fwup.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/policy_engine_snk.o: ../../../../Source/PD3_0/policy_engine_snk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_snk.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_snk.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/policy_engine_snk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/policy_engine_snk.o.d" -o ${OBJECTDIR}/_ext/840851172/policy_engine_snk.o ../../../../Source/PD3_0/policy_engine_snk.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/policy_engine_src.o: ../../../../Source/PD3_0/policy_engine_src.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_src.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_src.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/policy_engine_src.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/policy_engine_src.o.d" -o ${OBJECTDIR}/_ext/840851172/policy_engine_src.o ../../../../Source/PD3_0/policy_engine_src.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/policy_manager.o: ../../../../Source/PD3_0/policy_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_manager.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_manager.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/policy_manager.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/policy_manager.o.d" -o ${OBJECTDIR}/_ext/840851172/policy_manager.o ../../../../Source/PD3_0/policy_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/portpower_control.o: ../../../../Source/PD3_0/portpower_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/portpower_control.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/portpower_control.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/portpower_control.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/portpower_control.o.d" -o ${OBJECTDIR}/_ext/840851172/portpower_control.o ../../../../Source/PD3_0/portpower_control.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/protocol_layer.o: ../../../../Source/PD3_0/protocol_layer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/protocol_layer.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/protocol_layer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/protocol_layer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/protocol_layer.o.d" -o ${OBJECTDIR}/_ext/840851172/protocol_layer.o ../../../../Source/PD3_0/protocol_layer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/typeC_control.o: ../../../../Source/PD3_0/typeC_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/typeC_control.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/typeC_control.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/typeC_control.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/typeC_control.o.d" -o ${OBJECTDIR}/_ext/840851172/typeC_control.o ../../../../Source/PD3_0/typeC_control.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/upd_hw.o: ../../../../Source/PD3_0/upd_hw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/upd_hw.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/upd_hw.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/upd_hw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/upd_hw.o.d" -o ${OBJECTDIR}/_ext/840851172/upd_hw.o ../../../../Source/PD3_0/upd_hw.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/upd_interrupts.o: ../../../../Source/PD3_0/upd_interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/upd_interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/upd_interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/upd_interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/upd_interrupts.o.d" -o ${OBJECTDIR}/_ext/840851172/upd_interrupts.o ../../../../Source/PD3_0/upd_interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
else
${OBJECTDIR}/_ext/1984496892/plib_clock.o: ../src/config/default/peripheral/clock/plib_clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1984496892" 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1984496892/plib_clock.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1984496892/plib_clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1984496892/plib_clock.o.d" -o ${OBJECTDIR}/_ext/1984496892/plib_clock.o ../src/config/default/peripheral/clock/plib_clock.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/60167341/plib_eic.o: ../src/config/default/peripheral/eic/plib_eic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/60167341" 
	@${RM} ${OBJECTDIR}/_ext/60167341/plib_eic.o.d 
	@${RM} ${OBJECTDIR}/_ext/60167341/plib_eic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/60167341/plib_eic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/60167341/plib_eic.o.d" -o ${OBJECTDIR}/_ext/60167341/plib_eic.o ../src/config/default/peripheral/eic/plib_eic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1865468468/plib_nvic.o: ../src/config/default/peripheral/nvic/plib_nvic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1865468468" 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865468468/plib_nvic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1865468468/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1865468468/plib_nvic.o ../src/config/default/peripheral/nvic/plib_nvic.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o: ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1593096446" 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/1593096446/plib_nvmctrl.o ../src/config/default/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1865521619/plib_port.o: ../src/config/default/peripheral/port/plib_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1865521619" 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1865521619/plib_port.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1865521619/plib_port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1865521619/plib_port.o.d" -o ${OBJECTDIR}/_ext/1865521619/plib_port.o ../src/config/default/peripheral/port/plib_port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o: ../src/config/default/peripheral/sercom/spim/plib_sercom0_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1955867357" 
	@${RM} ${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o.d" -o ${OBJECTDIR}/_ext/1955867357/plib_sercom0_spi.o ../src/config/default/peripheral/sercom/spim/plib_sercom0_spi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o: ../src/config/default/peripheral/sercom/usart/plib_sercom3_usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/504274921" 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o.d" -o ${OBJECTDIR}/_ext/504274921/plib_sercom3_usart.o ../src/config/default/peripheral/sercom/usart/plib_sercom3_usart.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/829342655/plib_tc0.o: ../src/config/default/peripheral/tc/plib_tc0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/829342655" 
	@${RM} ${OBJECTDIR}/_ext/829342655/plib_tc0.o.d 
	@${RM} ${OBJECTDIR}/_ext/829342655/plib_tc0.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/829342655/plib_tc0.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/829342655/plib_tc0.o.d" -o ${OBJECTDIR}/_ext/829342655/plib_tc0.o ../src/config/default/peripheral/tc/plib_tc0.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/163028504/xc32_monitor.o: ../src/config/default/stdio/xc32_monitor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/163028504" 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/163028504/xc32_monitor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/163028504/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/163028504/xc32_monitor.o ../src/config/default/stdio/xc32_monitor.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1171490990/initialization.o: ../src/config/default/initialization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/initialization.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1171490990/initialization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1171490990/initialization.o.d" -o ${OBJECTDIR}/_ext/1171490990/initialization.o ../src/config/default/initialization.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1171490990/interrupts.o: ../src/config/default/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1171490990/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1171490990/interrupts.o.d" -o ${OBJECTDIR}/_ext/1171490990/interrupts.o ../src/config/default/interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1171490990/exceptions.o: ../src/config/default/exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1171490990/exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1171490990/exceptions.o.d" -o ${OBJECTDIR}/_ext/1171490990/exceptions.o ../src/config/default/exceptions.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1171490990/startup_xc32.o: ../src/config/default/startup_xc32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/startup_xc32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1171490990/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/1171490990/startup_xc32.o ../src/config/default/startup_xc32.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1171490990/libc_syscalls.o: ../src/config/default/libc_syscalls.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1171490990" 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1171490990/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/1171490990/libc_syscalls.o ../src/config/default/libc_syscalls.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o: ../../PowerDelivery/PSFSourceLite_App.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/318156199" 
	@${RM} ${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o.d 
	@${RM} ${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o.d" -o ${OBJECTDIR}/_ext/318156199/PSFSourceLite_App.o ../../PowerDelivery/PSFSourceLite_App.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o: ../../PowerDelivery/PSFSourceLite_BootCfg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/318156199" 
	@${RM} ${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o.d 
	@${RM} ${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o.d" -o ${OBJECTDIR}/_ext/318156199/PSFSourceLite_BootCfg.o ../../PowerDelivery/PSFSourceLite_BootCfg.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1671623073/Drivers.o: ../../../../SOC_Portable/SAMD20/Drivers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1671623073" 
	@${RM} ${OBJECTDIR}/_ext/1671623073/Drivers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1671623073/Drivers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1671623073/Drivers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1671623073/Drivers.o.d" -o ${OBJECTDIR}/_ext/1671623073/Drivers.o ../../../../SOC_Portable/SAMD20/Drivers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1936174342/pb_helper.o: ../../../../Source/PB/pb_helper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1936174342" 
	@${RM} ${OBJECTDIR}/_ext/1936174342/pb_helper.o.d 
	@${RM} ${OBJECTDIR}/_ext/1936174342/pb_helper.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1936174342/pb_helper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1936174342/pb_helper.o.d" -o ${OBJECTDIR}/_ext/1936174342/pb_helper.o ../../../../Source/PB/pb_helper.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1936174342/pb_mngr.o: ../../../../Source/PB/pb_mngr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1936174342" 
	@${RM} ${OBJECTDIR}/_ext/1936174342/pb_mngr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1936174342/pb_mngr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1936174342/pb_mngr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1936174342/pb_mngr.o.d" -o ${OBJECTDIR}/_ext/1936174342/pb_mngr.o ../../../../Source/PB/pb_mngr.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/debug.o: ../../../../Source/PD3_0/debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/debug.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/debug.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/debug.o.d" -o ${OBJECTDIR}/_ext/840851172/debug.o ../../../../Source/PD3_0/debug.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/int_globals.o: ../../../../Source/PD3_0/int_globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/int_globals.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/int_globals.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/int_globals.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/int_globals.o.d" -o ${OBJECTDIR}/_ext/840851172/int_globals.o ../../../../Source/PD3_0/int_globals.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/pd_main.o: ../../../../Source/PD3_0/pd_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/pd_main.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/pd_main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/pd_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/pd_main.o.d" -o ${OBJECTDIR}/_ext/840851172/pd_main.o ../../../../Source/PD3_0/pd_main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/pd_timer.o: ../../../../Source/PD3_0/pd_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/pd_timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/pd_timer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/pd_timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/pd_timer.o.d" -o ${OBJECTDIR}/_ext/840851172/pd_timer.o ../../../../Source/PD3_0/pd_timer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/policy_engine.o: ../../../../Source/PD3_0/policy_engine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/policy_engine.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/policy_engine.o.d" -o ${OBJECTDIR}/_ext/840851172/policy_engine.o ../../../../Source/PD3_0/policy_engine.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o: ../../../../Source/PD3_0/policy_engine_fwup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o.d" -o ${OBJECTDIR}/_ext/840851172/policy_engine_fwup.o ../../../../Source/PD3_0/policy_engine_fwup.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/policy_engine_snk.o: ../../../../Source/PD3_0/policy_engine_snk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_snk.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_snk.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/policy_engine_snk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/policy_engine_snk.o.d" -o ${OBJECTDIR}/_ext/840851172/policy_engine_snk.o ../../../../Source/PD3_0/policy_engine_snk.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/policy_engine_src.o: ../../../../Source/PD3_0/policy_engine_src.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_src.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_engine_src.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/policy_engine_src.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/policy_engine_src.o.d" -o ${OBJECTDIR}/_ext/840851172/policy_engine_src.o ../../../../Source/PD3_0/policy_engine_src.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/policy_manager.o: ../../../../Source/PD3_0/policy_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_manager.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/policy_manager.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/policy_manager.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/policy_manager.o.d" -o ${OBJECTDIR}/_ext/840851172/policy_manager.o ../../../../Source/PD3_0/policy_manager.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/portpower_control.o: ../../../../Source/PD3_0/portpower_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/portpower_control.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/portpower_control.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/portpower_control.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/portpower_control.o.d" -o ${OBJECTDIR}/_ext/840851172/portpower_control.o ../../../../Source/PD3_0/portpower_control.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/protocol_layer.o: ../../../../Source/PD3_0/protocol_layer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/protocol_layer.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/protocol_layer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/protocol_layer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/protocol_layer.o.d" -o ${OBJECTDIR}/_ext/840851172/protocol_layer.o ../../../../Source/PD3_0/protocol_layer.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/typeC_control.o: ../../../../Source/PD3_0/typeC_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/typeC_control.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/typeC_control.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/typeC_control.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/typeC_control.o.d" -o ${OBJECTDIR}/_ext/840851172/typeC_control.o ../../../../Source/PD3_0/typeC_control.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/upd_hw.o: ../../../../Source/PD3_0/upd_hw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/upd_hw.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/upd_hw.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/upd_hw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/upd_hw.o.d" -o ${OBJECTDIR}/_ext/840851172/upd_hw.o ../../../../Source/PD3_0/upd_hw.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/840851172/upd_interrupts.o: ../../../../Source/PD3_0/upd_interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/840851172" 
	@${RM} ${OBJECTDIR}/_ext/840851172/upd_interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/840851172/upd_interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/840851172/upd_interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/840851172/upd_interrupts.o.d" -o ${OBJECTDIR}/_ext/840851172/upd_interrupts.o ../../../../Source/PD3_0/upd_interrupts.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../PowerDelivery" -I"../../../../SOC_Portable/SAMD20" -I"../../../../Source/include" -I"../../../../Source/PB" -I"../../../../SOC_Portable/SAMD20/I2C_DC_DC" -ffunction-sections -fdata-sections -Os -DnCONFIG_HOOK_DEBUG_MSG -I"../src" -I"../src/config/default" -I"../src/packs/ATSAMD20E16_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/arm" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wno-packed-bitfield-compat 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PSF_EVB_Source_Lite.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION) -mno-device-startup-code -o dist/${CND_CONF}/${IMAGE_TYPE}/PSF_EVB_Source_Lite.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    -Wno-packed-bitfield-compat $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=512,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/PSF_EVB_Source_Lite.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -mno-device-startup-code -o dist/${CND_CONF}/${IMAGE_TYPE}/PSF_EVB_Source_Lite.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    -Wno-packed-bitfield-compat $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=512,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml 
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/PSF_EVB_Source_Lite.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
