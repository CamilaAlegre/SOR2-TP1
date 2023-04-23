.PHONY: all clean

SRC = read_boot read_mbr read_root read_restore read_delete read_root_files
BIN = $(SRC:.c=)


clean:
		rm -f $(BIN) $(OBJ)

all:
		gcc read_boot.c -o read_boot
		gcc read_mbr.c -o read_mbr
		gcc read_root.c -o read_root
		gcc read_restore.c -o read_restore
		gcc read_delete.c -o read_delete
		gcc read_root_files.c -o read_root_files
