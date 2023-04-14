#include <stdio.h>
#include <stdlib.h>

//Particion
typedef struct {
    unsigned char first_byte;
    unsigned char start_chs[3];
    unsigned char partition_type;
    unsigned char end_chs[3];
    char start_sector[4];
    char length_sectors[4];
} __attribute((packed)) PartitionTable;

//Para leer datos del boot sector
typedef struct {
    unsigned char jmp[3];
    char oem[8];
    unsigned short sector_size; // 2bytes
	
    // {...}  COMPLETAR
    unsigned char sectorsPerCluster; // 1byte
    unsigned short sizeReservedArea; // 2bytes
    unsigned char numberOfFats; // 1byte
    unsigned short maxFilesInRootDirectory; // 2bytes
    unsigned short numberOfSectorsFS; // 2bytes
    unsigned char mediaType; //b1byte
    unsigned short sizeOfEachFat;// 2bytes
    unsigned short sectorsPerTrack; // 2bytes
    unsigned short numbersOfHead; // 2bytes
    unsigned int numberOfSectorsBeforeStartPartition; // 4bytes
    unsigned int numberOfSectorsFSExtended; // 4bytes
    unsigned char bios13h; // 1byte
    char notUsed; // 1byte
    char bootSignature; // 1byte

    unsigned int volume_id;


    char volume_label[11];
    char fs_type[8]; // Type en ascii
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

int main() {
    FILE * in = fopen("test.img", "rb");
    int i, partitionStart = 446;
    PartitionTable pt[4];
    Fat12BootSector bs;
    
    fseek(in, partitionStart , SEEK_SET); // Ir al inicio de la tabla de particiones. Completar ...
    fread(pt, sizeof(PartitionTable), 4, in); // leo entradas 
    
    for(i=0; i<4; i++) {
        printf("Partiion type: %d\n", pt[i].partition_type);

        if(pt[i].partition_type == 1) {
            printf("Encontrado FAT12 %d\n", i);
            break;
        }
    }
    
    if(i == 4) {
        printf("No se encontró filesystem FAT12, saliendo ...\n");
        return -1;
    }
    
    //Vuelve al inicio
    fseek(in, 0, SEEK_SET);

    fread(&bs, sizeof(Fat12BootSector), 1, in);
    
    printf("  Jump code: %02X:%02X:%02X\n", bs.jmp[0], bs.jmp[1], bs.jmp[2]);
    printf("  OEM code: [%.8s]\n", bs.oem);
    printf("  sector_size: %d\n", bs.sector_size);

    printf("  Sectors per cluster: %i\n", bs.sectorsPerCluster);
    printf("  Size of reserved area, in sectors: %i\n", bs.sizeReservedArea);
    printf("  Number Of Fats: %i\n", bs.numberOfFats);
    printf("  Maximum number of files in the root directory : %i\n", bs.maxFilesInRootDirectory);
    printf("  Number of sectors in the file system: %i\n", bs.numberOfSectorsFS);
    printf("  Media Type (0xF0=removable disk, 0xF8=fixed disk): 0x%02X\n", bs.mediaType);
    printf("  Size of each FAT, in sectors: %i\n", bs.sizeOfEachFat);
    printf("  Sectors per track in storage device : %i\n", bs.sectorsPerTrack);
    printf("  Numbers of Head: %i\n", bs.numbersOfHead);
    printf("  Number Of Sectors Before Start Partition: %u\n", bs.numberOfSectorsBeforeStartPartition);
    printf("  Number Of Sectors in FS (Extended - 0 if not used): %i \n", bs.numberOfSectorsFSExtended);
    printf("  BIOS INT 13h (low level disk services) drive number: %02X \n", bs.bios13h);
    printf("  Extended boot signature: 0x%02X \n", bs.bootSignature);

    printf("  volume_id: 0x%08X\n", bs.volume_id);
    printf("  Volume label: [%.11s]\n", bs.volume_label);
    printf("  Filesystem type: [%.8s]\n", bs.fs_type);
    printf("  Boot sector signature: 0x%04X\n", bs.boot_sector_signature);
    
    fclose(in);
    return 0;
}
