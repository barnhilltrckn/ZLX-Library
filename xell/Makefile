CROSS=xenon-
CC=$(CROSS)gcc
OBJCOPY=$(CROSS)objcopy
LD=$(CROSS)ld
AS=$(CROSS)as
STRIP=$(CROSS)strip

LV1_DIR=xell/lv1

# Configuration
CFLAGS = -Wall -Os -I$(LV1_DIR) -ffunction-sections -fdata-sections \
	-m64 -mno-toc -DBYTE_ORDER=BIG_ENDIAN -mno-altivec -D$(CURRENT_TARGET)

AFLAGS = -Iinclude -m64
LDFLAGS = -nostdlib -n -m64 -Wl,--gc-sections

OBJS =	$(LV1_DIR)/startup.o \
	$(LV1_DIR)/main.o \
	$(LV1_DIR)/cache.o \
	$(LV1_DIR)/ctype.o \
	$(LV1_DIR)/string.o \
	$(LV1_DIR)/time.o \
	$(LV1_DIR)/vsprintf.o \
	$(LV1_DIR)/puff/puff.o

TARGETS = xell-1f xell-2f xell-gggggg

# Build rules
all: $(foreach name,$(TARGETS),$(addprefix $(name).,build))

.PHONY: clean %.build

clean:
	@echo Cleaning...
	@$(MAKE) --no-print-directory -f Makefile_lv2.mk clean
	@rm -rf $(OBJS) $(foreach name,$(TARGETS),$(addprefix $(name).,bin elf)) stage2.elf32.gz

%.build:
	@echo Building $* ... $(CURDIR)
	@$(MAKE) --no-print-directory -f $(CURDIR)/xell/Makefile $*.bin

.c.o:
	@echo [$(notdir $<)]
	@$(CC) $(CFLAGS) -c -o $@ $*.c

.S.o:
	@echo [$(notdir $<)]
	@$(CC) $(AFLAGS) -c -o $@ $*.S

xell-gggggg.elf: CURRENT_TARGET = HACK_GGGGGG
xell-1f.elf xell-2f.elf: CURRENT_TARGET = HACK_JTAG

%.elf: $(LV1_DIR)/%.lds $(OBJS)
	@$(CC) -n -T $< $(LDFLAGS) -o $@ $(OBJS)

%.bin: %.elf 
	@$(OBJCOPY) -O binary $< $@
