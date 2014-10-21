<?php

function term_dependency_settings_page(){
 
  $vocabulary = taxonomy_get_vocabularies();
  $checklist_vocab_array = array(); /* Change to array('0' => '--none--'); if you want a none option*/
  foreach ($vocabulary as $item) {
    $key = $item->vid;
    $value = $item->name;
    $checklist_vocab_array[$key] = $value;
  }

  $form['term_dependency_vocabularies'] = array(
    '#type'             => 'checkboxes',
    '#title'            => t('Select available vocabularie for term dependency'),
    '#position'         => 'left' ,
    '#options'          => $checklist_vocab_array ,
    '#default_value'    => variable_get('term_dependency_vocabularies'),
  );
  
  $types = node_type_get_types();
  ksort($types);
  $options=array();
  foreach ($types as $type => $propertie) {
    $options [$type] = $propertie->name;
  } 
  $form['term_dependency_content_types'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('Select available content type for term dependency'),
    '#default_value' => variable_get('term_dependency_content_types'),
  );
  
  return system_settings_form($form);
}
