#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define TAILLE 16

int rcon[10] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};


uint8_t Sbox[256] = {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

void afficher_tab(uint8_t tab[TAILLE]) {
  for (int32_t i = 0; i < TAILLE; i += 1) {
    printf("0x%.2x  ", tab[i]);
  }
  printf("\n");
  return;
}



void key_schedule(uint8_t key[16], uint8_t keys[11][16]){
  for (int32_t i = 0; i < 16; i += 1) {
    keys[0][i] = key[i];
  }

  for (int32_t k = 1 ; k < 11 ; k += 1) {
    keys[k][0] = Sbox[keys[k - 1][13]] ^ rcon[k - 1] ^ keys[k - 1][0];
    keys[k][1] = Sbox[keys[k - 1][14]] ^ keys[k - 1][1];
    keys[k][2] = Sbox[keys[k - 1][15]] ^ keys[k - 1][2];
    keys[k][3] = Sbox[keys[k - 1][12]] ^ keys[k - 1][3];
    for (int32_t i = 4 ; i < 16 ; i += 1) {
      keys[k][i] = keys[k - 1][i] ^ keys[k][i - 4];
    }
  }
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

void gen_sb_ark(uint8_t oct_key, uint8_t  Mask_in, uint8_t Mask_out) { 
  printf("\t");
  for (uint8_t i = 0; i < 255; i++) {
    printf("0x%.2x, ", Sbox[i ^ oct_key ^ Mask_in] ^ Mask_out);
  }
  printf("0x%.2x  ", Sbox[255 ^ oct_key ^ Mask_in] ^ Mask_out);
}

uint8_t demask_MC(uint8_t etat[], int r, uint8_t Mask[11][16]) {
  uint8_t tmp[16];
  //printf("Je suis dans le demask et r = %d.\n", r );
  for (int32_t i = 0; i < 16; i += 1) {
    tmp[i] = Mask[r + 1][i];

  }
  shift_row(tmp);
  mix_column(tmp);
  for (int32_t i = 0; i < 16; i += 1) {
    etat[i] ^= tmp[i];  
    }
    return 0;
}


int main(){

  uint8_t Mask[11][16]; /* Choisi aléatoirement */
  uint8_t etat[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t key[16]  = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t keys[10][16];

  uint8_t * boites[10][16];
    
  // Entrée du premier tour : pas de masque !
  for (int i = 0; i < 16; i++)
    Mask[0][i] = 0;

  // Entrée du r-ème tour : masque alétoire
  for (int r = 1; r < 10; r++) {
    for (int i = 0; i < 16; i++)
      Mask[r][i] = rand() & 0xff;
  }

  // Sortie du dernier tour : pas de masque !
  for (int i = 0; i < 16; i++)
    Mask[10][i] = 0;
    
  key_schedule(key, keys);

  printf("uint8_t Mask[11][16] = {\n");
  for (int r = 0; r < 11; r++) {
    printf("\t{ ");
    for (int i = 0; i < 16; i++)
      printf("0x%02x, ", Mask[r][i]);
    printf("},\n");
  }
  printf("};\n\n");
	    
   uint8_t tmp[16];  
  for (int r = 0; r < 10; r++) {
    // génère les boites du tour r

    for (int32_t i = 0; i < 16; i += 1) {
         tmp[i] = Mask[r][i];
    }
    shift_row(tmp);
    mix_column(tmp);
    for (int i = 0; i < 16; i++) {
      printf("uint8_t arksb%1x%1x[256] = {\n",r,i);
      //gen_sb_ark(keys[r][(i % 4) * 4 + (i / 4)], 0, 0);
      //if (r != 0) 

     
  //printf("Je suis dans le demask et r = %d.\n", r );
   

      //if (r != 0)
        gen_sb_ark(keys[r][(i % 4) * 4 + (i / 4)], tmp[i], Mask[r+1][i]);
     /* if (r == 0)
        gen_sb_ark(keys[r][(i % 4) * 4 + (i / 4)], 0, Mask[r+1][i]);
  */
     /* if (r != 0)
        demask_MC(etat, r, Mask);
      gen_sb_ark(keys[r][(i % 4) * 4 + (i / 4)], 0, Mask[r+1][i]);
      */
      printf("};\n");
      printf("\n\n");
    }
  }
//mc(sr(mask)) // demask

  printf("void initialize_boites(uint8_t * boites[10][16])\n");
  printf("{\n");
  for (int r= 0; r < 10; r++)
    for (int i = 0; i < 16; i++)
      printf("\tboites[%d][%d] = arksb%1x%1x;\n",r,i, r, i);
  printf("}\n");
    
  return 0;
}


