<?php




define('GUID_ACTION','action');   /* machine name du type de contenu point */
define('GUID_PROJECT','projet');   /* machine name du type de contenu projet */
define('GUID_PROP','proposition');   /* machine name du type de contenu proposition */

define('DEFAULT_TID_ADVANCEMENT',11 ); /*tid advancement 'work in progresse' */
define('DEFAULT_TID_WHEIGHTING', 14 ); /*tid wheighting 'souhaité' */


/*definition des types de contenu*/
define('CT_INTENTION','projet');
define('CT_ACTIVITE','activite');
define('CT_ORGANISME','organisme');

//define('CT_GROUPETRAVAIL','groupe_de_travail');
define('CT_GROUPETRAVAIL','cercle');

define('CT_ACTION','action');
define('CT_ARTICLE','article');
define('CT_DECISION','oeuvre');

define('CT_EKOPOINT','ekopoint');
define('CT_PROPOSITION','proposition');

/*definition des field collection*/
define('FC_PROP','field_col_proposition_1');
define('FC_POINT','field_col_point_1');
define('FC_TAXO','field_col_taxo_1');
define('FC_EKOPOINT','field_col_ekopoint_1');

define('FIELD_CUR_PROJECT','field_current_project');
define('FIELD_CUR_POINT','field_current_point');

/*definition des champs*/
// TODO : find and remove duplicate
define('FIELD_NID_ACTION','field_nid_action');
define('FIELD_ADVANC','field_tid_statut_1');
define('FIELD_NID_PROP','field_nid_proposition_1');
define('FIELD_POND','field_tid_ponderation_1');
define('FIELD_NIVEAU','field_tid_niveau_1');
define('FIELD_ETAPE','field_tid_etape_1');
define('FIELD_OBJECT','field_tid_objets');
define('FIELD_ACTION','field_tid_actions');
define('FIELD_DOMAINE','field_tid_domaines');
define('FIELD_FORMATED_DOMAINE','field_formatter_1');

define('FIELD_DOMAINE_MULTIPLE','field_tid_domaines');
define('FIELD_ACTION_MULTIPLE','field_tid_actions');
define('FIELD_OBJET_MULTIPLE','field_tid_objets');

define('FIELD_DOMAINE_UNIQUE','field_tid_domaines');
define('FIELD_ACTION_UNIQUE','field_tid_actions');
define('FIELD_OBJET_UNIQUE','field_tid_objets');

define('FIELD_DOMAINE_PROP','field_tid_domaines');
define('FIELD_ACTION_PROP','field_tid_actions');
define('FIELD_OBJET_PROP','field_tid_objets');

define('FIELD_DOMAINE_EKOPOINT','field_tid_domaines');
define('FIELD_ACTION_EKOPOINT','field_tid_actions');
define('FIELD_OBJET_EKOPOINT','field_tid_objets');

define('FIELD_REF_POINT_PROP', 'field_ref_prop_to_eko'); //ancien : field_nid_ekopoints
define('FIELD_REF_PROP_EKOPOINT', 'field_ref_eko_to_prop'); // ancien : field_nid_propositions

define('FIELD_COL_EKOPOINT','field_col_ekopoint');
define('FIELD_COL_EKOPROP','field_col_ekoprop');

define('FIELD_REF_PROP_FC_EKOPROP', 'field_entity_ref_prop'); //ancien : field_node_ref_prop
define('FIELD_REF_POINT_FC_EKOPOINT', 'field_entity_ref_ekopoint'); //ancien :field_node_ref_ekopoint




//defini le nom des vocabulaires
define('OBJET','objet');
define('ETAPE','etape');
define('NIVEAU','niveau');
define('ACTION','action');
define('DOMAINE','domaine');

//define('VID_ETAPE','3');
//define('VID_NIVEAU','2');
define('VID_ADVANCEMENT',4);     /* vid of vocabularie's advencement */



$fields_to_matrice=array(FIELD_NIVEAU,FIELD_ETAPE,FIELD_OBJECT,FIELD_ACTION,FIELD_DOMAINE);
