<?php

/**
 * admin/config/ekomundi page callback.
 */
function ekomundi_config($form, $form_state) {

  $types = node_type_get_types();
  ksort($types);
  $options=array();
  //get all content type group
  $content_group_types = array();
  $group_types = array();
  $all_types = array();
  foreach ($types as $type => $propertie) {
    $all_types[$type] = $propertie->name;
    if(og_is_group_content_type('node',$type)){
      $content_group_types[$type] = $propertie->name;
    }
    if(og_is_group_type('node',$type)){
      $group_types[$type] = $propertie->name;
    }
  }
  $form['ekomundi_config']=array(
    '#type' => 'vertical_tabs',
  );

  $form['ct_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t(''),
    '#group' => 'ekomundi_config'
  );

  $form['ct_form']['ekomundi_ekopoint_type'] = array(
    '#type' => 'select',
    '#options' => $all_types,
    '#title' => t('Select what content type is for ekopoint'),
    '#default_value' => variable_get('ekomundi_ekopoint_type'),
  );

  $form['ct_form']['ekomundi_proposition_type'] = array(
    '#type' => 'select',
    '#options' => $all_types,
    '#title' => t('Select what content type is for proposition'),
    '#default_value' => variable_get('ekomundi_proposition_type'),
  );

  $form['ct_form']['ekomundi_allowed_ekopoint_types'] = array(
    '#type' => 'checkboxes',
    '#options' => $content_group_types,
    '#title' => t('Select what content type can have ekopoint'),
    '#default_value' => variable_get('ekomundi_allowed_ekopoint_types'),

  );

  $form['ct_form']['ekomundi_parent_ekopoint_types'] = array(
    '#type' => 'checkboxes',
    '#options' => $group_types,
    '#title' => t('Select what parent content type can have ekopoint'),
    '#default_value' => variable_get('ekomundi_parent_ekopoint_types'),
  );


  $form['taxo_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t(''),
    '#group' => 'ekomundi_config'
  );


  $vocabularies= taxonomy_get_vocabularies(); //get all vocalbularies
  $option_voc=array();

  foreach($vocabularies as $vocabulary){
    $option_voc[$vocabulary->machine_name] =  $vocabulary->name ;
  }

  ksort($option_voc);

  $form['taxo_form']['ekomundi_voc_etape'] = array(
    '#type' => 'select',
    '#options' => $option_voc,
    '#title' => t('Select what vocabularie for <i>etape</i>'),
    '#default_value' => variable_get('ekomundi_voc_etape'),
  );
  $form['taxo_form']['ekomundi_voc_niveau'] = array(
    '#type' => 'select',
    '#options' => $option_voc,
    '#title' => t('Select what vocabularie for <i>niveau</i>'),
    '#default_value' => variable_get('ekomundi_voc_niveau'),
  );
  $form['taxo_form']['ekomundi_voc_objet'] = array(
    '#type' => 'select',
    '#options' => $option_voc,
    '#title' => t('Select what vocabularie for <i>objet</i>'),
    '#default_value' => variable_get('ekomundi_voc_objet'),
  );
  $form['taxo_form']['ekomundi_voc_action'] = array(
    '#type' => 'select',
    '#options' => $option_voc,
    '#title' => t('Select what vocabularie for <i>action</i>'),
    '#default_value' => variable_get('ekomundi_voc_action'),
  );
  $form['taxo_form']['ekomundi_voc_domaine'] = array(
    '#type' => 'select',
    '#options' => $option_voc,
    '#title' => t('Select what vocabularie for <i>domaine</i>'),
    '#default_value' => variable_get('ekomundi_voc_domaine'),
  );
  $form['taxo_form']['ekomundi_voc_ponderation'] = array(
    '#type' => 'select',
    '#options' => $option_voc,
    '#title' => t('Select what vocabularie for <i>ponderation</i>'),
    '#default_value' => variable_get('ekomundi_voc_ponderation'),
  );






  $field_options = array();
  $fields = field_info_fields();
  //dpm($fields);
  $term_ref_ekopoint = $term_ref_prop =array();
  $entity_ref_ekopoint = $entity_ref_prop = array();
  $field_cols = $entity_ref_fc =array();
  foreach ($fields as $field_name => $field_info) {

    if(in_array('taxonomy_term', $field_info['bundles'])){
      $field_for_taxo[$field_name] = $field_name;
    }
    if($field_info['type'] == 'taxonomy_term_reference'){
      $term_ref[$field_name] = $field_name;
    }
    if(isset($field_info['bundles']['node']) && $field_info['type'] == 'taxonomy_term_reference' && in_array(variable_get('ekomundi_ekopoint_type',''), $field_info['bundles']['node'])){
      $term_ref_ekopoint[$field_name] = $field_name;
    }
    if(isset($field_info['bundles']['node']) && $field_info['type'] == 'taxonomy_term_reference' && in_array(variable_get('ekomundi_proposition_type',''), $field_info['bundles']['node'])){
      $term_ref_prop[$field_name] = $field_name;
    }
    if(isset($field_info['bundles']['node']) && $field_info['type'] == 'entityreference' && in_array(variable_get('ekomundi_ekopoint_type',''), $field_info['bundles']['node'])){
      $entity_ref_ekopoint[$field_name] = $field_name;
    }
    if(isset($field_info['bundles']['node']) && $field_info['type'] == 'entityreference' && in_array(variable_get('ekomundi_proposition_type',''), $field_info['bundles']['node'])){
      $entity_ref_prop[$field_name] = $field_name;
    }
    if(isset($field_info['bundles']['field_collection_item']) && $field_info['type'] == 'entityreference' ){
      $entity_ref_fc[$field_name] = $field_name;
    }
    if( $field_info['type'] == 'field_collection' ){
      $field_cols[$field_name] = $field_name;
    }
  }
  $form['fc_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field collection'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t(''),
    '#group' => 'ekomundi_config'
  );
  $form['fc_form']['ekomundi_field_col_ekopoint'] = array(
    '#type' => 'select',
    '#options' => $field_cols,
    '#title' => t('Select what is the field collection for <i>ekopoint</i>'),
    '#default_value' => variable_get('ekomundi_field_col_ekopoint'),
  );
  $form['fc_form']['ekomundi_field_col_ekoprop'] = array(
    '#type' => 'select',
    '#options' => $field_cols,
    '#title' => t('Select what is the field collection for <i>ekoprop</i>'),
    '#default_value' => variable_get('ekomundi_field_col_ekoprop'),
  );

  $form['fields_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t(''),
    '#group' => 'ekomundi_config'
  );

  $form['fields_form']['ekomundi_field_etape'] = array(
    '#type' => 'select',
    '#options' => $term_ref,
    '#title' => t('Select what term reference for <i>etape</i>'),
    '#default_value' => variable_get('ekomundi_field_etape'),
  );
  $form['fields_form']['ekomundi_field_niveau'] = array(
    '#type' => 'select',
    '#options' => $term_ref,
    '#title' => t('Select what term reference for <i>niveau</i>'),
    '#default_value' => variable_get('ekomundi_field_niveau'),
  );
  $form['fields_form']['ekomundi_field_objet'] = array(
    '#type' => 'select',
    '#options' => $term_ref,
    '#title' => t('Select what term reference for <i>objet</i>'),
    '#default_value' => variable_get('ekomundi_field_objet'),
  );
  $form['fields_form']['ekomundi_field_domaine'] = array(
    '#type' => 'select',
    '#options' => $term_ref,
    '#title' => t('Select what term reference for <i>domaine</i>'),
    '#default_value' => variable_get('ekomundi_field_domaine'),
  );
  $form['fields_form']['ekomundi_field_action'] = array(
    '#type' => 'select',
    '#options' => $term_ref,
    '#title' => t('Select what term reference for <i>action</i>'),
    '#default_value' => variable_get('ekomundi_field_action'),
  );

  $form['fields_form']['ekopoints_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields for ekopoint'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t(''),
  );
  $form['fields_form']['ekopoints_fields']['ekomundi_ekopoint_field_objet'] = array(
    '#type' => 'select',
    '#options' => $term_ref_ekopoint,
    '#title' => t('Select what term reference for <i>objet</i>'),
    '#default_value' => variable_get('ekomundi_ekopoint_field_objet'),
  );
  $form['fields_form']['ekopoints_fields']['ekomundi_ekopoint_field_domaine'] = array(
    '#type' => 'select',
    '#options' => $term_ref_ekopoint,
    '#title' => t('Select what term reference for <i>domaine</i>'),
    '#default_value' => variable_get('ekomundi_ekopoint_field_domaine'),
  );
  $form['fields_form']['ekopoints_fields']['ekomundi_ekopoint_field_action'] = array(
    '#type' => 'select',
    '#options' => $term_ref_ekopoint,
    '#title' => t('Select what term reference for <i>action</i>'),
    '#default_value' => variable_get('ekomundi_ekopoint_field_action'),
  );
  $form['fields_form']['ekopoints_fields']['ekomundi_ekopoint_field_ref_prop'] = array(
    '#type' => 'select',
    '#options' => $entity_ref_ekopoint,
    '#title' => t('Select what entityreference references <i>proposition</i>'),
    '#default_value' => variable_get('ekomundi_ekopoint_field_ref_prop'),
  );

  $form['fields_form']['proposition_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields for proposition'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t(''),
  );
  $form['fields_form']['proposition_fields']['ekomundi_prop_field_etape'] = array(
    '#type' => 'select',
    '#options' => $term_ref_prop,
    '#title' => t('Select what term reference for <i>etape</i>'),
    '#default_value' => variable_get('ekomundi_prop_field_etape'),
  );
  $form['fields_form']['proposition_fields']['ekomundi_prop_field_niveau'] = array(
    '#type' => 'select',
    '#options' => $term_ref_prop,
    '#title' => t('Select what term reference for <i>niveau</i>'),
    '#default_value' => variable_get('ekomundi_prop_field_niveau'),
  );
  $form['fields_form']['proposition_fields']['ekomundi_prop_field_objet'] = array(
    '#type' => 'select',
    '#options' => $term_ref_prop,
    '#title' => t('Select what term reference for <i>objet</i>'),
    '#default_value' => variable_get('ekomundi_prop_field_objet'),
  );
  $form['fields_form']['proposition_fields']['ekomundi_prop_field_domaine'] = array(
    '#type' => 'select',
    '#options' => $term_ref_prop,
    '#title' => t('Select what term reference for <i>domaine</i>'),
    '#default_value' => variable_get('ekomundi_prop_field_domaine'),
  );
  $form['fields_form']['proposition_fields']['ekomundi_prop_field_action'] = array(
    '#type' => 'select',
    '#options' => $term_ref_prop,
    '#title' => t('Select what term reference for <i>action</i>'),
    '#default_value' => variable_get('ekomundi_prop_field_action'),
  );
  $form['fields_form']['proposition_fields']['ekomundi_prop_field_ref_ekopoint'] = array(
    '#type' => 'select',
    '#options' => $entity_ref_prop,
    '#title' => t('Select what entityreference references <i>ekopoint</i>'),
    '#default_value' => variable_get('ekomundi_prop_field_ref_ekopoint'),
  );

  $form['fields_form']['fc_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields for field collection'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t(''),
  );
  $form['fields_form']['fc_fields']['ekomundi_fc_ref_ekopoint'] = array(
    '#type' => 'select',
    '#options' => $entity_ref_fc,
    '#title' => t('Select what entityreference references the ekopoint in the <i>field collection ekopoint</i>'),
    '#default_value' => variable_get('ekomundi_fc_ref_ekopoint'),
  );
  $form['fields_form']['fc_fields']['ekomundi_fc_ref_ekoprop'] = array(
    '#type' => 'select',
    '#options' => $entity_ref_fc,
    '#title' => t('Select what entityreference references the proposition in the <i>field collection ekoprop</i>'),
    '#default_value' => variable_get('ekomundi_fc_ref_ekoprop'),
  );


  $form['defaultvalues_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default values'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t(''),
    '#group' => 'ekomundi_config'
  );

  $name_ponderation = taxonomy_vocabulary_machine_name_load(variable_get('ekomundi_voc_ponderation'));
  $vid_ponderation = $name_ponderation->vid;
  $taxo=taxonomy_get_tree($vid_ponderation);
  //creer les options pour le champ select du vocabulaire
  $term_array=array();

  foreach ($taxo as $term) {
    $tid = $term->tid;
    //$term_array[$tid] = str_repeat('-',$term->depth).i18n_taxonomy_term_name($term,$langcode);
    $term_array[$tid] = str_repeat('-',$term->depth).$term->name;
  }
  $form['defaultvalues_form']['ekomundi_dv_ponderation'] = array(
    '#type' => 'select',
    '#options' => $term_array,
    '#title' => t('Select what default value for the ponderation'),
    '#default_value' => variable_get('ekomundi_dv_ponderation'),
  );

  return system_settings_form($form);

}

function ekomundi_config_audience_group_form($form, $form_state) {

  $types = node_type_get_types();
  ksort($types);
  $options=array();
  //get all content type group
  $content_groups = array();
  foreach ($types as $type => $propertie) {
    if(og_is_group_type('node',$type)){
      $content_groups[$type] = $propertie->name;
    }
  }

  foreach ($types as $type => $propertie) {
    if(og_is_group_content_type('node',$type)){
      $form['ekomundi_audience_'.$type] = array(
        '#type' => 'checkboxes',
        '#options' => $content_groups,
        '#title' => t('Select available content type group for @group_name', array('@group_name' => $propertie->name)),
        '#default_value' => variable_get('ekomundi_audience_'.$type),
      );
    }
  }
  return system_settings_form($form);
}
