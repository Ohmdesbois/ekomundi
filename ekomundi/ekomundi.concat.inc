<?php


function ekomundi_concat($terms){
	global $language ;
	global $user;
	$langcode =  $language->language ;
	$output = '';

  $domaine = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_domaine'));
  $vid_domaine = $domaine->vid;
  $action = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_action'));
  $vid_action = $action->vid;
  $objet = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_objet'));
  $vid_objet = $objet->vid;

  $etape = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_etape'));
  $vid_etape = $etape->vid;
  $niveau = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_niveau'));
  $vid_niveau = $niveau->vid;

  if(!is_array($terms[$vid_action])){
    $tid = $terms[$vid_action];
    $terms[$vid_action] = array($tid);
  }
  if(!is_array($terms[$vid_domaine])){
    $tid = $terms[$vid_domaine];
    $terms[$vid_domaine] = array($tid);
  }
  if(is_array($terms[$vid_objet])){
    $tid = $terms[$vid_objet][0];
    $terms[$vid_objet] = $tid;
  }

  if(isset($terms[$vid_etape]) && isset($terms[$vid_niveau])){
    //recupere la conjugaison dans action suivant l'etape et le niveau.
    $etape = taxonomy_term_load($terms[$vid_etape]);
    //$niveau=taxonomy_term_load($terms[$vid_niveau]);

    $genre = $terms[$vid_niveau];
    $temps = $etape->field_txt_temps_1[$langcode]['0']['value'];
  }
  else{
    $genre = $temps = '';
  }

	$format_objet = '';
	$format_actions = '';

	foreach($terms[$vid_action] as $tid){

		$action=taxonomy_term_load($tid);
    if(empty($genre) && empty($temps) ){
      $conjugaison = '"' . $action->{'field_txt_term_format_2'}[$langcode]['0']['value'] . '" ';
    }
    else {
      $conjugaison = $action->{'field_txt_'.$temps.'_'.$genre.'_1'}[$langcode]['0']['value'];
    }


		//recupere la formatage de l'objet
		$objet = taxonomy_term_load($terms[$vid_objet]);
		$format_objet = $objet->field_txt_term_format_1[$langcode]['0']['value'];

		//remplace ce que suivant l'action
		if(trim(drupal_strtolower($format_objet)) == 'ce que'){
	 	//dpm($action);

			$field=str_replace(' ','_',trim(drupal_strtolower($format_objet)));
			//dpm($field);

			if(!empty($action->{'field_txt_'.$field.'_1'}[$langcode]['0']['value']) ){
				$format_objet = $action->{'field_txt_'.$field.'_1'}[$langcode]['0']['value'];
			}

			$format_after = $action->{'field_txt_'.$field.'_after_1'}[$langcode]['0']['value'];

		}
		else{
			$format_after = isset($action->field_txt_term_format_1[$langcode]) ? $action->field_txt_term_format_1[$langcode]['0']['value'] : '';

		}
		$format_actions .= trim($conjugaison.$format_after).', ';
	}
	$format_actions = drupal_substr($format_actions,0,-2).' ';


	$format_domaines = '';
	$first_domaine = true;
	foreach($terms[$vid_domaine] as $tid){
		//recupere la formatage du domaine
		$domaine = taxonomy_term_load($tid);
		$format_domaine = $domaine->field_txt_term_format_1[$langcode]['0']['value'];
		if(!$first_domaine)
			$format_domaine = str_replace('dans le domaine',' ',$format_domaine);

		$format_domaines .= $format_domaine.', ';
		$first_domaine = false;
	}
	$format_domaines = drupal_substr($format_domaines,0,-2);

	$output = $format_objet . $format_actions . trim($format_domaines) . '.';

/*
		$term=taxonomy_term_load($tid);
			//$voc_point[$vocabularies[$vid]->machine_name]=array('tid'=>$tid,'name'=>drupal_strtolower($term_det->name));
			$tname=i18n_taxonomy_term_name($term,$langcode);
			$voc_point[$vocabularies[$vid]->machine_name]=array('tid'=>$tid,'name'=>drupal_strtolower($tname));

	*/

	return $output;
}

