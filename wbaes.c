#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define TAILLE 16
#include "boites.h"


void afficher_tab(uint8_t tab[TAILLE]) {
    for (int32_t i = 0; i < TAILLE; i += 1) {
		printf("0x%.2x  ", tab[i]);
    }
    printf("\n");
    return;
}

void shift_row(uint8_t etat[16]) {
    uint8_t tmp[16] = {
		       etat[0],etat[1],etat[2],etat[3],
		       etat[5],etat[6],etat[7],etat[4],
		       etat[10],etat[11],etat[8],etat[9],
		       etat[15],etat[12],etat[13],etat[14]
    };
    for (int i = 0; i < 16; i += 1) {
		etat[i] = tmp[i];
    }
}


void mix_column(uint8_t etat[16]) {
    uint8_t tmp[16];
    for (int32_t i = 0; i < 4; i += 1) {
		uint8_t etat_i_2, etat_i_3;
		uint8_t etat_i4_2, etat_i4_3;
		uint8_t etat_i8_2, etat_i8_3;
		uint8_t etat_i12_2, etat_i12_3;
		unsigned char h; // Si h "vaut" 1 je fais le xor sinon non
	//printf("h = 0x%.2x \n",h);

		h = (unsigned char)((signed char) etat[i] >> 7);
		etat_i_2   = (etat[i] << 1) ^ 0x1b & h;

		h = (unsigned char)((signed char) etat[i + 4] >> 7);
		etat_i4_2  = (etat[i + 4] << 1) ^ 0x1b & h;

		h = (unsigned char)((signed char) etat[i + 8] >> 7);
		etat_i8_2  = (etat[i + 8] << 1) ^ 0x1b & h;

		h = (unsigned char)((signed char) etat[i + 12] >> 7);
		etat_i12_2  = (etat[i + 12] << 1) ^ 0x1b & h;

		   
		etat_i_3   = etat_i_2   ^ etat[i];
		etat_i4_3  = etat_i4_2  ^ etat[i + 4];
		etat_i8_3  = etat_i8_2  ^ etat[i + 8];
		etat_i12_3 = etat_i12_2 ^ etat[i + 12];
			   
	        
		tmp[i]      = etat_i_2    ^ etat_i4_3       ^ etat[i + 8] * 1 ^ etat[i + 12] * 1;
		tmp[i + 4]  = etat[i] * 1 ^ etat_i4_2       ^ etat_i8_3       ^ etat[i + 12] * 1;
		tmp[i + 8]  = etat[i] * 1 ^ etat[i + 4] * 1 ^ etat_i8_2       ^ etat_i12_3      ;
		tmp[i + 12] = etat_i_3    ^ etat[i + 4] * 1 ^ etat[i + 8] * 1 ^ etat_i12_2      ;
    }
    for (int32_t i = 0 ; i < 16 ; i += 1) {
		etat[i] = tmp[i];
    }
}


int main(){
    
   	uint8_t etat[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t * boites[10][16];
    initialize_boites(boites);

    for (int r = 0; r < 10; r++) {
		for (int i = 0; i < 16; i++) { 
	    	etat[i] = boites[r][i][etat[i]];
		}
		shift_row(etat);
		if (r != 9) {
	    	mix_column(etat);
		}
    }


    printf("*** Attendu *** \n");
    printf("0x0e  0xc6  0x45  0x14  0xdd  0x21  0x5b  0x18  0x33  0xe5  0xd8  0xbe  0xd3  0x46  0xba  0xc8\n");
    printf("*** Final ***\n");
    afficher_tab(etat);
    
    return 0;
}