################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
firmware/openos/openwsn/07-App/rinfo/%.o: ../firmware/openos/openwsn/07-App/rinfo/%.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"c:/ti/ccsv6/tools/compiler/gcc-arm-none-eabi-4_7-2013q3/bin/arm-none-eabi-gcc.exe" -c -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/cross-layers" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/02a-MAClow" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/02b-MAChigh" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/03a-IPHC" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/03b-IPv6" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/04-TRAN" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/layerdebug" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/ohlone" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/r6t" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/rex" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/rinfo" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/rleds" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/rreg" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/rrt" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/rwellknown" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/tcpecho" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/tcpprint" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/tcpinject" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/udpstorm" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/udprand" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/udpprint" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/udplatency" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/udpinject" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/udpecho" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn/07-App/swarmband" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/projects/common/03oos_openwsn" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/projects/cc2538em/03oos_openwsn" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/bsp/boards/cc2538em/inc" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/bsp/boards/cc2538em/source" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/bsp/boards/cc2538em/sens_itf" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/bsp/boards/cc2538em" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/bsp/boards" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/drivers" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/drivers/common" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/kernel/openos" -I"C:/OWSNMB/cc2538em-master/openwsn/firmware/openos/openwsn" -I"c:/ti/ccsv6/tools/compiler/gcc-arm-none-eabi-4_7-2013q3/arm-none-eabi/include" -g -gstrict-dwarf -Wall -mthumb -mcpu=cortex-m3 -g3 -O0 -Wstrict-prototypes -Wall -mlittle-endian -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


