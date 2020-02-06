
#include <stdio.h>
#include "accio.h"

int main(void) {

    RFILE *fp = NULL;
    int stat;
    int count;
    int length;
    int offset;
    int buflen;
    int type;
    int subtype;
    void *packet = NULL;
    char buffer[ACCBUF];
    ACR_FILENAME file;
    ACR_IMAGENAME image;
    ACR_PRINT print;
    ACR_RESOURCE resource;
    ACR_IDENT ident;
    ACR_USERDATA user;

    if ((fp = acc_open("")) != NULL) {

        while ((count = acc_read_record(fp, buffer, ACCBUF)) != -1) {

            if (acc_get_header(buffer, count, &buflen, &type, &subtype)) {

                offset = 0;

                switch (type) {
                    case ACR$K_FILE_BL:
                        offset = sizeof(ACR_HEADER);
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &file);
                        acc_free_record(type, subtype, &file);
                        acc_free_packet(type, subtype, packet);
                        break;

                    case ACR$K_FILE_FL:
                        offset = sizeof(ACR_HEADER);
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &file);
                        acc_free_record(type, subtype, &file);
                        acc_free_packet(type, subtype, packet);
                        break;

                    case ACR$K_IMGDEL:
                        offset = sizeof(ACR_HEADER);
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &ident);
                        acc_free_record(type, subtype, &ident);
                        acc_free_packet(type, subtype, packet);

                        offset += length;
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &resource);
                        acc_free_record(type, subtype, &resource);
                        acc_free_packet(type, subtype, packet);

                        offset += length;
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &image);
                        acc_free_record(type, subtype, &image);
                        acc_free_packet(type, subtype, packet);
                        break;

                    case ACR$K_LOGFAIL:
                        offset = sizeof(ACR_HEADER);
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &ident);
                        acc_free_record(type, subtype, &ident);
                        acc_free_packet(type, subtype, packet);

                        offset += length;
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &resource);
                        acc_free_record(type, subtype, &resource);
                        acc_free_packet(type, subtype, packet);
                        break;

                    case ACR$K_PRCDEL:
                        offset = sizeof(ACR_HEADER);
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &ident);
                        acc_free_record(type, subtype, &ident);
                        acc_free_packet(type, subtype, packet);

                        offset += length;
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &resource);
                        acc_free_record(type, subtype, &resource);
                        acc_free_packet(type, subtype, packet);
                        break;

                    case ACR$K_PRINT:
                        offset = sizeof(ACR_HEADER);
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &ident);
                        acc_free_record(type, subtype, &ident);
                        acc_free_packet(type, subtype, packet);

                        offset += length;
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &print);
                        acc_free_record(type, subtype, &print);
                        acc_free_packet(type, subtype, packet);
                        break;

                    case ACR$K_SYSINIT:
                        offset = sizeof(ACR_HEADER);
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &ident);
                        acc_free_record(type, subtype, &ident);
                        acc_free_packet(type, subtype, packet);

                        offset += length;
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &resource);
                        acc_free_record(type, subtype, &resource);
                        acc_free_packet(type, subtype, packet);
                        break;

                    case ACR$K_USER:
                        offset = sizeof(ACR_HEADER);
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &ident);
                        acc_free_record(type, subtype, &ident);
                        acc_free_packet(type, subtype, packet);

                        offset += length;
                        packet = acc_get_packet(buffer, buflen, offset, &length, &type, &subtype);
                        stat = acc_decode_packet(type, subtype, packet, &user);
                        acc_free_record(type, subtype, &user);
                        acc_free_packet(type, subtype, packet);
                        break;

                    default:
                        break;
                }

            }

        }

        acc_close(fp);

    }

}
    
