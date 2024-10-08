
#include "fr_util.h"
#include "timing.h"
#include "memory.h"

/*
 * Loads from virtual address addr and measure the access time
 */
CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
    CYCLES start, end;

    start = arm_v8_get_timing();
    arm_v8_access_memory(addr);
    end = arm_v8_get_timing();

    // asm volatile("mov %1, %%r8\n\t"
    //         "lfence\n\t"
    //         "rdtsc\n\t"
    //         "mov %%eax, %%edi\n\t"
    //         "mov (%%r8), %%r8\n\t"
    //         "lfence\n\t"
    //         "rdtsc\n\t"
    //         "sub %%edi, %%eax\n\t"
    // : "=a"(cycles) /*output*/
    // : "r"(addr)
    // : "r8", "edi");

    return end - start;
}

/* 
 * Returns Time Stamp Counter 
 */
// extern inline __attribute__((always_inline))
// CYCLES rdtscp(void) {
// 	CYCLES cycles;
// 	asm volatile ("rdtscp"
// 	: /* outputs */ "=a" (cycles));

// 	return cycles;
// }

/* 
 * Gets the value Time Stamp Counter 
 */
inline CYCLES get_time() {
    // return rdtscp();
    return arm_v8_get_timing();
}

/* Synchronizes at the overflow of a counter
 *
 * Counter is created by masking the lower bits of the Time Stamp Counter
 * Sync done by spinning until the counter is less than CHANNEL_SYNC_JITTER
 */
extern inline __attribute__((always_inline))
CYCLES cc_sync() {
    while((get_time() & CHANNEL_SYNC_TIMEMASK) > CHANNEL_SYNC_JITTER) {}
    return get_time();
}


/*
 * Flushes the cache block accessed by a virtual address out of the cache
 */
extern inline __attribute__((always_inline))
void clflush(ADDR_PTR addr)
{
    // asm volatile ("clflush (%0)"::"r"(addr));
    arm_v8_flush(addr);
}

/*
 * Convert a given ASCII string to a binary string.
 * From:
 * https://stackoverflow.com/questions/41384262/convert-string-to-binary-in-c
 */
char *string_to_binary(char *s)
{
    if (s == NULL) return 0; /* no input string */

    size_t len = strlen(s) - 1;

    // Each char is one byte (8 bits) and + 1 at the end for null terminator
    char *binary = malloc(len * 8 + 1);
    binary[0] = '\0';

    for (size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for (int j = 7; j >= 0; --j) {
            if (ch & (1 << j)) {
                strcat(binary, "1");
            } else {
                strcat(binary, "0");
            }
        }
    }

    return binary;
}

/*
 * Convert 8 bit data stream into character and return
 */
char *conv_char(char *data, int size, char *msg)
{
    for (int i = 0; i < size; i++) {
        char tmp[8];
        int k = 0;

        for (int j = i * 8; j < ((i + 1) * 8); j++) {
            tmp[k++] = data[j];
        }

        char tm = strtol(tmp, 0, 2);
        msg[i] = tm;
    }

    msg[size] = '\0';
    return msg;
}

/*
 * Prints help menu
 */
void print_help() {

	printf("-f,\tFile to be shared between sender/receiver\n"
		"-o,\tSelected offset into shared file\n"
		"-i,\tTime interval for sending a single bit\n");

}

/*
 * Parses the arguments and flags of the program and initializes the struct config
 * with those parameters (or the default ones if no custom flags are given).
 */
void init_config(struct config *config, int argc, char **argv)
{
	// Initialize default config parameters
	int offset = DEFAULT_FILE_OFFSET;
	config->interval = CHANNEL_DEFAULT_INTERVAL;
	char *filename = DEFAULT_FILE_NAME;


	// Parse the command line flags
	//      -f is used to specify the shared file 
	//      -i is used to specify the sending interval rate
	//      -o is used to specify the shared file offset
	int option;
	arm_v8_timing_init();
	while ((option = getopt(argc, argv, "i:o:f:")) != -1) {
		switch (option) {
			case 'i':
				config->interval = atoi(optarg);
				break;
			case 'o':
				offset = atoi(optarg)*CACHE_BLOCK_SIZE;
				break;
			case 'f':
				filename = optarg;
				break;
			case 'h':
				print_help();
				exit(1);
			case '?':
				fprintf(stderr, "Unknown option character\n");
				print_help();
				exit(1);
			default:
				print_help();
				exit(1);
		}
	}

	// Map file to virtual memory and extract the address at the file offset
	if (filename != NULL) {
		int inFile = open(filename, O_RDONLY);
		if(inFile == -1) {
			printf("Failed to Open File\n");
			exit(1);
		}

		void *mapaddr = mmap(NULL,DEFAULT_FILE_SIZE,PROT_READ,MAP_SHARED,inFile,0);

		if (mapaddr == (void*) -1 ) {
			printf("Failed to Map Address\n");
			exit(1);
		}

		config->addr = (ADDR_PTR) mapaddr + offset;
	}
}

