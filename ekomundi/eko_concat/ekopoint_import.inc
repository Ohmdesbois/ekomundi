<?php

function ekopoint_import_form($form, &$form_state,$add_to_nid) {
	global $language ;
	global $user;
	//$langcode = ($language->language=='fr')? 'und' : $language->language  ;
	$langcode =  $language->language ;

  $user=user_load($user->uid);

  $fc_ekpt = variable_get('ekomundi_field_col_ekopoint');
  $fc_ref_ekpt = variable_get('ekomundi_fc_ref_ekopoint');

  $query=db_select('node','n');
  $query->fields('n',array('nid','title'));
  $query->condition('n.type','intention','=');
  $query->condition('n.uid',$user->uid,'=');
  $result = $query->execute();

	$form['help']=array('#markup'=>'du texte...');

  while($record = $result->fetchAssoc()) {
    //champ fieldset du formaire
    $form['intention_'.$record['nid']] = array(
      '#type'  => 'fieldset',
      '#title'   => t($record['title']),
    );
    $list_ekopoint=array();
    $node=node_load($record['nid']);
    if(isset($node -> {$fc_ekpt}['und']) ){
      foreach($node -> {$fc_ekpt}['und'] as $delta => $item_id){
        $fc=entity_load('field_collection_item', array($item_id['value']));
        $fc=$fc[$item_id['value']];
        if(isset($fc->{$fc_ref_ekpt}['und'])){
          foreach($fc->{$fc_ref_ekpt}['und'] as $delta_fc => $ref){
            $node_ref=node_load($ref['nid']);

            $fe = variable_get('ekomundi_field_etape');
            $etape = taxonomy_term_load( $node_ref->{$fe}['und'][0]['tid']);
            $temps =$etape->field_txt_temps_1[$langcode]['0']['value'];

            $fn = variable_get('ekomundi_field_niveau');
            $niveau = taxonomy_term_load( $node_ref->{$fn}['und'][0]['tid']);
            $genre =$niveau->field_txt_temps_1[$langcode]['0']['value'];


            $fa = variable_get('ekomundi_field_action');
						$action = taxonomy_term_load($node_ref->{$fa}['und'][0]['tid']);
						$conjugaison = $action->{'field_txt_'.$temps.'_'.$genre.'_1'}['und']['0']['value'];

						$string = $node_ref->title;
						$pattern = '/(.*)"(.*)"(.*)/i';
						$replacement = '${1}'.trim(htmlspecialchars_decode($conjugaison, ENT_QUOTES )).'${3}';

						$title = preg_replace($pattern, $replacement, $string);

            $list_ekopoint[$item_id['value']]= t($title);
          }

        }
      }
			$form['intention_'.$record['nid']]['fieldcols_'.$record['nid']] = array(
				'#type' => 'checkboxes',
				//'#title' => t('Choose what sence to add in your project'),
				'#options' => $list_ekopoint,
        '#ajax' => array(
							'callback' => 'ajax_ekopoint_import_callback',
							'wrapper' => 'submit_import',
						)
			);

    }
  }
  $form['submit_import'] = array(
    '#prefix' => '<div id="submit_import">',
    '#suffix' => '</div>',
  );

	//si le formulaire a été envoyé via ajax
  $count_checked=0;
	if(isset($form_state['values'])){

    foreach($form_state['values'] as $id=>$value) {
			if(drupal_substr($id,0,10)=='fieldcols_') {
        foreach($value as $field_col_item){
          if($field_col_item!=0){$count_checked++;}
        }
      }
		}
    if($count_checked>=1){
      $form['submit_import']['submit_import_form'] = array(
        '#type' => 'submit',
        '#value' => t('Add to the project'),
      );
      $form['submit_import']['projectid'] = array(
        '#type' => 'hidden',
        '#value' => $add_to_nid,
      );
    }

	}// end of  if(isset($form_state['values']


  return $form;
}

function ajax_ekopoint_import_callback($form, &$form_state) {
	return $form['submit_import'];
}
function ekopoint_import_form_validate($form, &$form_state) {

}

function ekopoint_import_form_submit($form, &$form_state) {

  $to_copy=array();
  $node_project=node_load($form_state['values']['projectid']);

  $fc_ekpt = variable_get('ekomundi_field_col_ekopoint');
  $fc_ekpp = variable_get('ekomundi_field_col_ekoprop');
  $fc_ref_ekpt = variable_get('ekomundi_fc_ref_ekopoint');
  foreach($form_state['values'] as $id=>$value) {
    if(drupal_substr($id,0,10)=='fieldcols_') {
      foreach($value as $field_col_item){
        if($field_col_item!=0){
          $fc=entity_load('field_collection_item', array($field_col_item));
          $fc=$fc[$field_col_item];

          $already_in_project=FALSE;
          if(isset($node_project->{$fc_ekpt}[LANGUAGE_NONE])){
            foreach($node_project->{$fc_ekpt}[LANGUAGE_NONE] as $delta => $field_col_item_project ){
              $fc_project=entity_load('field_collection_item', array($field_col_item_project['value']));
              $fc_project=$fc_project[$field_col_item_project['value']];

              if($fc_project->{$fc_ref_ekpt}==$fc->{$fc_ref_ekpt}){  //old FIELD_REF_POINT_FC_EKOPOINT
                $already_in_project=TRUE;
              }
            }
          }

          $node_ref=node_load($fc->{$fc_ref_ekpt}[LANGUAGE_NONE][0]['nid']);

          if(!$already_in_project){
            $field_collection_item = entity_create('field_collection_item', array('field_name' => 'field_col_ekopoint'));
            $field_collection_item->setHostEntity('node', $node_project);
            $field_collection_item->{$fc_ref_ekpt} = $fc->{$fc_ref_ekpt};
            $field_collection_item->{$fc_ekpp}= $fc->{$fc_ekpp};
            $field_collection_item->save();

            drupal_set_message(t('<em>@title_ekopoint</em> was added in <em>@title_project</em>.',array('@title_ekopoint'=>$node_ref->title,'@title_project'=>$node_project->title)));
          }
          else{
            drupal_set_message(t('<em>@title_ekopoint</em> already exists in <em>@title_project</em>.',array('@title_ekopoint'=>$node_ref->title,'@title_project'=>$node_project->title)),'warning');
          }


          //$node_project->field_col_ekopoint[LANGUAGE_NONE];


        }
      }
    }
  }
  $form_state['redirect']  = 'node/'.$form_state['values']['projectid'];
  //node_save($node_project);


}
