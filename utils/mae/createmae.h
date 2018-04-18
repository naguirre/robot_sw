#ifndef __CREATE_MAE_H__
#define __CREATE_MAE_H__

CREATE_MAE_ENUM(MAE_NOM, MAE_LISTE_ETATS)

#undef  DERNIER_ETAT
#define DERNIER_ETAT(a)	 a ///< for function table

CREATE_MAE_FONCTIONS(MAE_NOM, MAE_LISTE_ETATS)

#undef  DERNIER_ETAT
#define DERNIER_ETAT(a)    ETAT_ ## a

#undef MAE_NOM
#undef MAE_LISTE_ETATS

#endif /*  __CREATE_MAE_H__ */
