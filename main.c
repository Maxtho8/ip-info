#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <math.h>
int verifier_format(char *ip);
int extraire_valeur(int number ,char *ip, int nb_char);
char verifier_classe(int number);
char verifier_visibilite(int *number, char classe);
void obtenir_reseau_hote(int *ip, int masque);


int main (void)
{
     // pour embellir l'affichage    
    printf("|--------------------------------------------------------------|\n");
    printf("| Veuillez entrer l'adresse IP au format : XXX.XXX.XXX.XXX/XX  |\n");
    printf("                        ");

    // récupérer l'ip entré par l'utilisateur 
    char *buffer = NULL;
    size_t taille_ligne = 0;
    int nb_char = 0;
    nb_char = getline(&buffer, &taille_ligne,stdin);


    printf("|--------------------------------------------------------------|\n");

    int valide = verifier_format(buffer); 
    // vérification des paramaètres renvoyé par la fonction
    if(valide == 0)
    {
        char *erreur	= "";
        printf("| L'adresse ne respecte pas le format demandé. Fin du programme|\n");
        printf("|--------------------------------------------------------------|\n");
        exit(0);

    }

    // creation d'un tableau pour contenir les nombres décomposés de l'ip     
    int ip_d[5];
    // remplir le tableau avec les valeurs de l'ip
    for( int i=0; i < 5;i++)
    {
        ip_d[i] = extraire_valeur(i,buffer,nb_char);
        if(ip_d[i] == -1){
             printf("| L'octet %d contient une valeur interdite (0 <= x <256)        |\n",i+1);
             printf("|--------------------------------------------------------------|\n");
            exit(0);
        }

    }

    char classe = verifier_classe(ip_d[0]);

    // la variable pointeur_ip_d est une convertion du tableau ip_d en pointeur, qui sera utilisé dans les prochaines fonctions
    int *pointeur_ip_d = ip_d;


    char visibilite = verifier_visibilite(pointeur_ip_d,classe); 
    printf("| Classe : %c                                                   |\n",classe);
    printf("|--------------------------------------------------------------|\n");

    // traitement des paramètres renvoyés par la fonction
    if(visibilite == 'x')
    {
        printf("| Visibilité : Privé\n",classe);
    }
    else if(visibilite == 'z')
    {
        printf("| Visibilité : Public                                          |\n",classe);

    }
    else
    {
         printf("| Impossible de déterminer la visibilité                       |\n",classe);
    }


    obtenir_reseau_hote(pointeur_ip_d,ip_d[4]);


    printf("|----------------------FIN DU PROGRAMME------------------------|\n");  

}



int verifier_format(char *ip)
{
   int err;
   regex_t preg;
   //char str_request[] = ip;
   const char *str_regex = "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\/[0-9]{1,2}";
   err = regcomp (&preg, str_regex, REG_NOSUB | REG_EXTENDED);
   if (err == 0)
   {
      int match;


      match = regexec (&preg, ip, 0, NULL, 0);
       // si la chaine correspond renvoie 1 sinon 0
	  if(match == 0)
	  {
			return 1;
	  }
	  else
	  {
		return 0;
	  }
   }
}
int extraire_valeur(int number ,char *ip,int nb_char)

{
	//on converti le pointeur ip dans un tableau.
	char tempstring[nb_char];
	for(int r = 0; r < nb_char; r++){
	tempstring[r] = *ip;
	ip++;
	}
	
	// variable temporaire pour sauvegarder l'indice
	int z=0;
	//on crée un tableau size contenant la taille de chaque nombres séparé par un "." ou "/" , exemple 192.16.256.8/8 size = ([0] => 3,[1] => 2,[2] => 3,[3] => 1,[4] => 1 )
	int size[5];
	// tableau qui sera renvoyé, contient l'ip décomposé sous la forme d'int
	int ip_d[5];
	// variable temporaire pour sauvegardé le nombre de chiffre avant le prochain "."
	int temp = 0;
	// boucle pour récupérer la taille des 3 premiers nombre de l'ip
	for(int i = 0;i < 3;i++){
		
		while(tempstring[z] != '.'){
			temp++;
			z++;
		}
		size[i] = temp;
		temp = 0;
		z++;
		
	}
	// boucle pour récupérer la taille du dernier octet de l'ip (celui avant le "/")
	while(tempstring[z] != '/'){
			temp++;
			z++;
		}
	size[3] = temp;
	temp = 0;
	z++;
    // récupération de la valeur du masque en faisant une soustraction 
	size[4] = (nb_char-1)-z;
	
    // chaine de caractère dans laquelle je stocke l'octet a convertir en int
	char tempo[3];
    // remise a 0 de la variable qui va contenir l'indice pour parcourir l'ip
	temp = 0;
    // pointeur pour la fonction endPtr
	char * endPtr;
    // en connaissant la taille de chaque octet je peux parcourir l'ip en evitant les points, puis je stocke dans la variable tempo qui sera converti en int
	for(int i =0; i < 5;i++){
		for(int k = 0; k < size[i];k++){
			tempo[k] = tempstring[temp];
			temp++;
		}
		temp++;
		ip_d[i] = strtol( tempo, &endPtr, 10 );
        // je vide le tableau temporaire 
		tempo[0] = NULL;
		tempo[1] = NULL;
		tempo[2] = NULL;
		
		
	}
    //on verifie si l'octet demandé par l'utilisateurs est compris entre 255 et 0 
	if(ip_d[number] < 256 && ip_d[number] >= 0 ) {
	return ip_d[number];
	}
	else {
	return -1;
	}
	
}


char verifier_classe(int number)
{
        
        if (!(number & (1 << 7))) {
            return 'A';
        }
        else 
        {
            if (!(number & (1 << 6))){
                return 'B';
            }
            else 
            {
				if (!(number & (1 << 5))){
                return 'C';
				}
				else {
						if (!(number & (1 << 4))){
						return 'D';
						}
						else {
							return 'E';
						}

				}
				
				
			}
        }
     
  
 }
 char verifier_visibilite(int *number, char classe)
{
     // retourne x si privé et z si publique et retourne e en cas d'erreur 
	
	if(classe == 'A') {
       if(*number == 10)  {
           return 'x';
       }
       else  {
           return 'z';
       }
        
    }
    else if(classe == 'B'){
        int bit0 = *number;
        number++;
        int bit1 = *number;
        
        if(bit0 == 172 && bit1 >= bit1 && bit1 <= 31){
            return 'x';
        }
        else {
            return 'z';
        }
    }
    else if (classe == 'C'){
        int bit0 = *number;
        number++;
        int bit1 = *number;
        number++;
        int bit2 = *number;
        
        if(bit0 == 192 && bit1 == 168 && bit2 >= 1 ) {
            return 'x';
        }
        else {
            return 'z';
        }
    }
    else {
        return 'e';
    }
}
void obtenir_reseau_hote(int *ip,int masque)
{
    // garder uniquement la partie entière
    float temp_division = masque/8;
    int division = temp_division;
    // obtenir l'excédent de bit
    int excedent = masque%8;
    // verifie si le masque vaut 8 , 16 ou 24
    if(excedent == 0 && division >= 1)
    {
      printf(" Partie réseau : ");
      for(int i = 1; i<=4;i++){
          if(i <= division){
              printf("%d",*ip);
              ip++;
          }
          else{
              printf("0");
          }
          if( i != 4 ){
              printf(".");
          }
          else
              printf("\r\n"); 
      }
    }
    // sinon je calcule la valeur du bit excédent et je fais un ET logique avec le bit
    else
    {
      int z = 0;
      printf("Adresse réseau : ");
      for(int i = 1; i<=division;i++){
          printf("%d",*ip);
          ip++;
          if( i != division ){
              printf(".");
          }
          z++;
      }
        // calcul de la valeur de l'excedent en convertissant l'excedent de bit en un nombre, par exemple /18 => 255.255.(1*2^7 + 1*2^6).0 => 255.255.192.0
      int valeur_excedent = 0;
      for(int i=0;i < excedent;i++){
          valeur_excedent += pow(2,(7-i));    
      }
      printf(".%d",(*ip&valeur_excedent));
      z++;
        
    // pour compléter l'affichage  l'adresse reseau
       if(masque < 24 && masque >= 16){
          printf(".0");
      }
      else if(masque < 8 && masque >= 1){
          printf("Le masque de sous reseaux est invalide \r\n");
          exit(0);
      }
      else if(masque < 16 && masque >= 8){
          printf(".0.0");
      }
      else if( masque > 31){
           printf("Le masque de sous reseaux est invalide \r\n");
          exit(0);
          
      }
      printf("\r\n");
      
    }
    
}
