#include <stdio.h>
#include <stdlib.h>


typedef struct {
    unsigned char first_byte;
    unsigned char start_chs[3];
    unsigned char partition_type;
    unsigned char end_chs[3];
    char starting_cluster[4];
    char file_size[4];
} __attribute((packed)) PartitionTable;

typedef struct {
    unsigned char jmp[3];
    char oem[8];
    unsigned short sector_size;

    unsigned char sectorsPerCluster;
    unsigned short reserved_sectors;
    unsigned char number_of_fats;
    unsigned short root_dir_entries;
    unsigned short numberOfSectorsFS;
    unsigned char mediaType;
    unsigned short fat_size_sectors;
    unsigned short sectorsPerTrack;
    unsigned short numbersOfHead;
    unsigned int numberOfSectorsBeforeStartPartition;
    unsigned int numberOfSectorsFSExtended;
    unsigned char bios13h;
    char notUsed;
    char bootSignature;

    unsigned int volume_id;
    char volume_label[11];
    char fs_type[8];
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

//Estructura de 32 bytes
typedef struct {
    unsigned char filename[1];
    unsigned char name[7];
    unsigned char extension[3];
    unsigned char attributes[1];
    unsigned char reserved;
    unsigned char created_time_seconds;
    unsigned char created_time_hours_minutes_seconds[2];
    unsigned char created_day[2];
    unsigned char accessed_day[2];
    unsigned char cluster_highbytes_address[2];
    unsigned char written_time[2];
    unsigned char written_day[2];
    unsigned short cluster_lowbytes_address;
    unsigned int size_of_file;
} __attribute((packed)) Fat12Entry;


void restore_delete(Fat12Entry *entry, FILE *in, int start_file) {

    FILE * in2 = fopen("test.img", "r+");

    //Muestro nombre y extension
    printf("Archivo que comienza con 0xE5: [%c%.8s.%.3s]\n", 0xE5, entry->filename, entry->extension);

    fseek(in2, start_file, SEEK_SET);
    entry->filename[0] = 'a'; //Seteo cualquier letra que reemplace 0xE5
    fwrite(&entry, sizeof(entry), 1, in2);

    fclose(in2);
}

int main() {
    FILE * in = fopen("test.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    Fat12Entry entry;
    int partitionStart = 446; //Inicio de particion
   
    fseek(in, partitionStart, SEEK_SET);
    fread(pt, sizeof(PartitionTable), 4, in);  
   
    fseek(in, 0, SEEK_SET);
    fread(&bs, sizeof(Fat12BootSector), 1, in);
   
           
    fseek(in, (bs.reserved_sectors-1 + bs.fat_size_sectors * bs.number_of_fats) *
          bs.sector_size, SEEK_CUR);
   
    printf("Root dir_entries %d \n", bs.root_dir_entries);

    for(i=0; i<bs.root_dir_entries; i++) {
        fread(&entry, sizeof(entry), 1, in);

        //Primer byte del nombre
        if(entry.filename[0] == 0xE5) {

            int start_file = ftell(in) - sizeof(entry);
            restore_delete(&entry, in, start_file);
        }
    }
   
    fclose(in);
    return 0;
}