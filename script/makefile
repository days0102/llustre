
######################################
##  make example

# Define variables
IP := $(shell ip addr show | grep -oP '(?<=inet\s)\d+(\.\d+){3}' | grep -v '127.0.0.1' \
	| tr '\n' ' ' | awk '{print $$1}')
LOOP_DEVICES := /dev/loop30 /dev/loop31 /dev/loop32
IMG_FILES := lustre-mgt.img lustre-mdt.img lustre-ost.img
MOUNT_POINTS := mnt/lustre/mgt mnt/lustre/mdt mnt/lustre/ost mnt/lustre/client

# Default target
example: check_ip configure mount

# Check IP address
check_ip:
	@if [ -z "$(IP)" ]; then \
		echo "empty ip_address"; \
		exit 1; \
	fi
	@cnt=$$(echo "$(IP)" | tr -s ' ' '\n' | wc -l); \
	if [ $$cnt -ne 1 ]; then \
		echo "too many ip_address"; \
		exit 1; \
	fi
	@echo "ip_address: $(IP)"

configure: build_loop mkfs


# Create loop devices image
build_images: 
	@echo "build images..."
	sudo dd if=/dev/zero of=lustre-mgt.img bs=1M count=256
	sudo dd if=/dev/zero of=lustre-mdt.img bs=1M count=1024
	sudo dd if=/dev/zero of=lustre-ost.img bs=1M count=1024

# Set loop devices
build_loop:
	@echo "start set loop device..."
	sudo losetup /dev/loop30 lustre-mgt.img
	sudo losetup /dev/loop31 lustre-mdt.img
	sudo losetup /dev/loop32 lustre-ost.img

remove_loop:
	@echo "remove loop device..."
	sudo losetup -d /dev/loop30 || true
	sudo losetup -d /dev/loop31 || true
	sudo losetup -d /dev/loop32 || true

# Create Lustre file systems
mkfs:
	@echo "start mkfs.lustre..."
	sudo mkfs.lustre --mgs /dev/loop30
	sudo mkfs.lustre --mdt --mgsnode=$(IP)@tcp --fsname=lustrefs --index=0 /dev/loop31
	sudo mkfs.lustre --ost --mgsnode=$(IP)@tcp --fsname=lustrefs --index=0 /dev/loop32

remkfs:
	@echo "start mkfs.lustre..."
	sudo mkfs.lustre --mgs --reformat /dev/loop30
	sudo mkfs.lustre --mdt --mgsnode=$(IP)@tcp --reformat --fsname=lustrefs --index=0 /dev/loop31
	sudo mkfs.lustre --ost --mgsnode=$(IP)@tcp --reformat --fsname=lustrefs --index=0 /dev/loop32

# Mount file systems
mount: mount_mgt mount_mdt mount_ost mount_client

mount_mgt:
	@echo "start mount mgt..."
	sudo mkdir -p mnt/lustre/mgt
	sudo mount -t lustre /dev/loop30 mnt/lustre/mgt

mount_mdt:
	@echo "start mount mdt..."
	sudo mkdir -p mnt/lustre/mdt
	sudo mount -t lustre /dev/loop31 mnt/lustre/mdt

mount_ost:
	@echo "start mount ost..."
	sudo mkdir -p mnt/lustre/ost
	sudo mount -t lustre /dev/loop32 mnt/lustre/ost

mount_client:
	@echo "start mount client..."
	sudo mkdir -p mnt/lustre/client
	sudo mount -t lustre $(IP)@tcp:/lustrefs mnt/lustre/client

# Clean up
remove:
	@echo "cleaning up..."
	sudo umount mnt/lustre/client || true
	sudo umount mnt/lustre/ost || true
	sudo umount mnt/lustre/mdt || true
	sudo umount mnt/lustre/mgt || true
	sudo losetup -d /dev/loop30 || true
	sudo losetup -d /dev/loop31 || true
	sudo losetup -d /dev/loop32 || true
	#sudo rm -f lustre-mgt.img lustre-mdt.img lustre-ost.img

debug_mkfs:
	sudo losetup -d /dev/loop25 || true
	sudo dd if=/dev/zero of=lustre.img bs=1M count=64
	sudo losetup /dev/loop25 lustre.img
	sudo cp /usr/lib/lustre/mount_osd_ldiskfs.so /usr/lib/lustre/mount_osd_ldiskfs.so-backup
	sudo cp ../lustre/utils/mount_osd_ldiskfs.so /usr/lib/lustre/


debug_mount:
	sudo losetup -d /dev/loop25 || true
	sudo dd if=/dev/zero of=lustre.img bs=1M count=64
	sudo losetup /dev/loop25 lustre.img
	sudo mkfs.lustre --mgs /dev/loop25
	# @sudo mkfs.lustre --mdt --mgsnode=$(IP)@tcp --fsname=lustrefs --index=0 /dev/loop31
	# @sudo mkfs.lustre --ost --mgsnode=$(IP)@tcp --fsname=lustrefs --index=0 /dev/loop32

debug_clean:
	sudo umount mnt || true

.PHONY: check_ip build_images configure mkfs mount mount_mgt mount_mdt mount_ost mount_client remove

#####################################

wbc:
	sudo lctl set_param llite.*.wbc.conf="conf flush_mode=aging_keep max_inodes=200000 size=4G" 

wbc_disable:
	sudo lctl set_param llite.*.wbc.conf="disabled" 

lhsm_enable:
	sudo lhsmtool_posix --daemon --hsm-root /home/lwz/pcc --archive=2 /home/lwz/script/mnt/lustre/client

pcc_add:
	sudo lctl pcc add /home/lwz/script/mnt/lustre/client /home/lwz/pcc -p "uid={1000} rwid=2"


# 1. --earse-param set to delete the origin param XXXXX NOT Worked
# 2. --write-conf set to write the new param XXXXX NOT Worked

# 1. change the MGS NID by --earse-param and --write-conf
# 2. mount MGS
# 3. sudo lctl replace_nids lustrefs-MDT0000 192.168.203.2@tcp
# 4. sudo lctl replace_nids lustrefs-OST0000 192.168.203.2@tcp
# 5. umount MGS
# 6. mount lustre

tunefs:
	sudo tunefs.lustre --erase-param --mgsnode=192.168.203.2@tcp --writeconf	/dev/loop30
	sudo mount -t lustre /dev/loop30 mnt/lustre/mgt
	sudo lctl replace_nids lustrefs-OST0000 192.168.203.2@tcp
	sudo lctl replace_nids lustrefs-OST0000 192.168.203.2@tcp
	# sudo tunefs.lustre --erase-param --mgsnode=192.168.203.2@tcp --writeconf /dev/loop30
	# sudo tunefs.lustre --erase-param --mgsnode=192.168.203.5@tcp /dev/loop31
	# sudo tunefs.lustre --erase-param --mgsnode=192.168.203.2@tcp --writeconf --mgsnode=192.168.203.2@tcp /dev/lo1
	# sudo tunefs.lustre --erase-param --mgsnode=192.168.203.5@tcp /dev/loop32
	# sudo tunefs.lustre --writeconf --mgsnode=192.168.203.2@tcp /dev/loop32
