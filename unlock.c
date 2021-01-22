/*
   Bruteforce Bootloader Unlocker for HUAWEI Related smartphones
   This is intended to be the fastest ever created bootloader unlocker
   */
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "usb.h"

#define LEN 37
//#define DEBUG

unsigned long long base_start = 1000000000000000;
static unsigned short vendor_id = 0;
static const char *serial = 0;

void resumer(){
    printf("\n\nLast used code was: %lld\n", base_start);
    FILE * fp = fopen("lastcode","w");
    fprintf(fp,"%llu",base_start);
    fclose(fp);
    exit(1);
}


//********Fast itoa obtained from https://github.com/jeaiii/itoa**********
struct pair { char t, o; };
#define P(T) T, '0',  T, '1', T, '2', T, '3', T, '4', T, '5', T, '6', T, '7', T, '8', T, '9'
static const struct pair s_pairs[] = { P('0'), P('1'), P('2'), P('3'), P('4'), P('5'), P('6'), P('7'), P('8'), P('9') };

#define W(N, I) *(struct pair*)&b[N] = s_pairs[I]
#define A(N) t = ((uint64_t)(1) << (32 + N / 5 * N * 53 / 16)) / (uint32_t)(1e##N) + 1 + N/6 - N/8, t *= u, t >>= N / 5 * N * 53 / 16, t += N / 6 * 4, W(0, t >> 32)
#define S(N) b[N] = (char)((uint64_t)(10) * (uint32_t)(t) >> 32) + '0'
#define D(N) t = (uint64_t)(100) * (uint32_t)(t), W(N, t >> 32)

#define C0 b[0] = (char)(u) + '0'
#define C1 W(0, u)
#define C2 A(1), S(2)
#define C3 A(2), D(2)
#define C4 A(3), D(2), S(4)
#define C5 A(4), D(2), D(4)
#define C6 A(5), D(2), D(4), S(6)
#define C7 A(6), D(2), D(4), D(6)
#define C8 A(7), D(2), D(4), D(6), S(8)
#define C9 A(8), D(2), D(4), D(6), D(8)

#define L09(F) u < 100        ? L01(F) : L29(F)
#define L29(F) u < 1000000    ? L25(F) : L69(F)
#define L25(F) u < 10000      ? L23(F) : L45(F)
#define L69(F) u < 100000000  ? L67(F) : L89(F)
#define L03(F) u < 100        ? L01(F) : L23(F)

#define L01(F) u < 10         ? F(0) : F(1)
#define L23(F) u < 1000       ? F(2) : F(3)
#define L45(F) u < 100000     ? F(4) : F(5)
#define L67(F) u < 10000000   ? F(6) : F(7)
#define L89(F) u < 1000000000 ? F(8) : F(9)

#define POS(N) (N < length ? C##N, N + 1 : N + 1)
#define NEG(N) (N + 1 < length ? *b++ = '-', C##N, N + 2 : N + 2)

size_t to_chars_jeaiii(char* b, size_t length, uint64_t n)
{
    size_t count;
    uint32_t u = (uint32_t)(n);
    uint64_t t;

    if (u == n)
        return L09(POS);

    uint64_t a = n / 100000000;

    if ((uint32_t)(a) == a)
    {
        u = (uint32_t)(a);
        b += count = L09(POS);
        count += 8;
        if (count > length)
            return count;
    }
    else
    {
        u = (uint32_t)(a / 100000000);
        b += count = L03(POS);
        count += 16;
        if (count > length)
            return count;

        u = a % 100000000;
        C7;
        b += 8;
    }

    u = n % 100000000;
    C7;
    return count;
}
//********End of Fast itoa obtained from https://github.com/jeaiii/itoa**********



//*****Original code from fastboot****** Retrieved from : https://android.googlesource.com/platform/system/core/+/eclair-release/fastboot/fastboot.c
int match_fastboot(usb_ifc_info *info)
{
    if(!(vendor_id && (info->dev_vendor == vendor_id)) &&
	    (info->dev_vendor != 0x18d1) &&  // Google
	    (info->dev_vendor != 0x0451) &&
	    (info->dev_vendor != 0x22b8) &&  // Motorola
	    (info->dev_vendor != 0x413c) &&  // DELL
	    (info->dev_vendor != 0x0bb4))    // HTC
	return -1;
    if(info->ifc_class != 0xff) return -1;
    if(info->ifc_subclass != 0x42) return -1;
    if(info->ifc_protocol != 0x03) return -1;
    // require matching serial number if a serial number is specified
    // at the command line with the -s option.
    if (serial && strcmp(serial, info->serial_number) != 0) return -1;
    return 0;
}

usb_handle *open_device(void)
{
    static usb_handle *usb = 0;
    int announce = 1;

    if(usb) return usb;

    for(;;) {
	usb = usb_open(match_fastboot);
	if(usb) return usb;
	if(announce) {
	    announce = 0;    
	    fprintf(stderr,"< waiting for device >\n");
	}
	sleep(1);
    }
}
//*****End of original code from fastboot******




inline __attribute__((always_inline))
int bruteforce(usb_handle* usb, const char* cmd, size_t size)
{
    u_int8_t status[64];
    int r;
    if(usb_write(usb, cmd, size) != size)
    {
	fprintf(stderr, "Failed to send oem command for %s\n", cmd);
	return -1;
    }
    r = usb_read(usb, status,64);
    if(memcmp(status, "OKAY", 4))
    {
	#ifdef DEFINE 
	fprintf(stderr, "Received error from fastboot : %.*s\n",r, status);
	#endif
	return -1;
    }
    printf("okay\n");
    return 1;
}


int main( int argc, char **argv) {
    usb_handle *usb = 0;
    usb = open_device();
    if ( argc > 1 ) {
	char *base = argv[1];
	base_start = atoll( base );
    } else {
	FILE * fp;
	if(fp= fopen("lastcode","rw")) {
	    fscanf(fp,"%llu",&base_start);
	    fclose(fp);
	}
    }
    printf("-- Bootloader unlocker/bruteforcer for huawei devices --\nIt might take from several hours to days to unlock your device, so be patient. Sit back, relax and do your stuff, I will inform you once its done.\n"); 
    signal(SIGINT, resumer);
    signal(SIGTERM, resumer);
    char fou[LEN] = "oem unlock ";
    int dignum, ret;
    do
    {
	dignum = to_chars_jeaiii(fou + 11, 22, base_start);
	fou[11 + dignum] = 0;
	ret = bruteforce(usb, fou, 11 + dignum);
	base_start++;
	#ifdef DEBUG
	printf("Trying key : %s\n", fou);
	#endif
    }while(dignum && ret);

    printf("Your unlock code is: %llu", base_start);

    usb_close(usb);
    return 1;
}
