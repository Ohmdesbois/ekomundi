<?php

function eko_current_node_settings_page(){

  $types = node_type_get_types();
  ksort($types);
  $options=array();
  foreach ($types as $type => $propertie) {
    $options [$type] = $propertie->name;
  } 
  $form['eko_current_node_content_types'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('Select available content type for current node'),
    '#default_value' => variable_get('eko_current_node_content_types'),
  );
  
  return system_settings_form($form);
}
