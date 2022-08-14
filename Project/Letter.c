#include "Letter.h"

#define SPACES 30

//From "Menu.c"
extern xpm_image_t background_frame;

xpm_image_t alphabet[26];

void (load_all_letters)(){
  for(int i = 0; i < 26; i++)
    load_letter('A' + i);
}

void (load_letter)(char letter){
  switch(letter){
    case 'A': xpm_load(A_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'B': xpm_load(B_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'C': xpm_load(C_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'D': xpm_load(D_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'E': xpm_load(E_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'F': xpm_load(F_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'G': xpm_load(G_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'H': xpm_load(H_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'I': xpm_load(I_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'J': xpm_load(J_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'K': xpm_load(K_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'L': xpm_load(L_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'M': xpm_load(M_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'N': xpm_load(N_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'O': xpm_load(O_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'P': xpm_load(P_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'Q': xpm_load(Q_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'R': xpm_load(R_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'S': xpm_load(S_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'T': xpm_load(T_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'U': xpm_load(U_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'V': xpm_load(V_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'W': xpm_load(W_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'X': xpm_load(X_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'Y': xpm_load(Y_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'Z': xpm_load(Z_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;

    default: 
      break;
  }
}

void (draw_letter)(uint16_t x, uint16_t y, char letter){
  uint32_t *bytes = (uint32_t*) alphabet[letter - 65].bytes;

  for(uint16_t i = 0; i < alphabet[letter - 65].width; i++){
    for(uint16_t j = 0; j < alphabet[letter - 65].height; j++){
      if(*(bytes + i + j * alphabet[letter - 65].width) != TRANSPARENCY)
        paint_pixel(x + i, y + j, *(bytes + i + j * alphabet[letter - 65].width));
    }
  }

  insert_letter_into_back_frame(x, y, letter);
}

void (draw_string)(char *text, uint16_t x, uint16_t y){
  for(size_t i = 0; i < strlen(text); i++)
    draw_letter(x + i*SPACES, y, text[i]);

  insert_string_into_back_frame(x, y, text);
}

void (insert_letter_into_back_frame)(uint16_t x, uint16_t y, char letter){
  uint32_t *bytes = (uint32_t*)alphabet[letter - 65].bytes;
  uint32_t *back_frame = (uint32_t*)background_frame.bytes;

  for(uint16_t i = 0; i < alphabet[letter - 65].width; i++){
    for(uint16_t j = 0; j < alphabet[letter - 65].height; j++){
      if(*(bytes + i + j * alphabet[letter - 65].width) != TRANSPARENCY)
        *(back_frame + (x + i) + (y + j)*background_frame.width) = *(bytes + i + j*alphabet[letter - 65].width);
    }
  }
}

void (insert_string_into_back_frame)(uint16_t x, uint16_t y, char *text){
  for(size_t i = 0; i < strlen(text); i++)
    insert_letter_into_back_frame(x + i*SPACES, y, text[i]);
}

