<?php

/**
 * admin/config/ekomundi page callback.
 */
function ekomundi_config($form, $form_state) {




  //This is my ajax trigger element
  $form['element_trigger'] = array(
    '#type' => 'select',
    '#options' => array(
      'one' => 'one',
      'two' => 'two',
      'three' => 'three',
    ),
    '#ajax' => array(
      'callback' => 'ajax_example_simplest_callback',

      /** Q: Can I somehow declare more than one wrapper? **/
      //Say for instance, something like:
      'wrapper' => array('replace_div_1', 'replace_div_2'),

     ),
  );

  //replace_div_1
  $form['element_to_be_replaced_1'] = array(
    '#type' => 'textfield',
    '#title' => t("My conditional field one"),
    '#prefix' => '<div id="replace_div_1">',
    '#suffix' => '</div>',
  );


 //... more form elements here

  //replace_div_2
  $form['element_to_be_replaced_2'] = array(
    '#type' => 'textfield',
    '#title' => t("My conditional field two"),
    '#prefix' => '<div id="replace_div_2">',
    '#suffix' => '</div>',
  );


  $regions=array('r1','r2','r3');

  /**********************************
  *  content type
  */
  $form['ekomundi_ct']=array(
    '#type' => 'fieldset',
    '#group' => 'packages',
    '#title' => t('Content type Allowed'),
  );

  //get all content type
  $query = db_select('node_type', 'n');
  $query->fields('n',array('type','name'))//SELECT the fields from node_type
  ->orderBy('name', 'ASC');//ORDER BY name
  $result = $query->execute();

  $option_ct=array();
  while($record = $result->fetchAssoc()) {
    $option_ct[$record['type']]=$record['name'];
  }
  //ajoute ekopoint comme un type de contenu
  $option_ct['ekopoint']='eKopoint';

  foreach($regions as  $region){
    $form['ekomundi_ct']['eko_ct_'.$region]=array(
      '#title' => t('Content type allowed for '.$region),
      '#type' => 'checkboxes',
      '#options' => $option_ct,
      '#description' => t('Allow users to use this content type for this region.'),
      '#default_value' => variable_get('eko_ct_'.$region,(array)''),

    );
  }

  /**********************************
  *  vocabularies
  */
  $form['ekomundi_voc']=array(
    '#type' => 'fieldset',
    '#group' => 'packages',
    '#title' => t('Vocabularies Allowed'),
  );

  $vocabularies= taxonomy_get_vocabularies(); //get all vocalbularies
  $config_vocab=variable_get('eko_config_voc'); //get all the config vocalbularies allowed

  $option_voc=array();

  //merge the config to a vocabulary to make vocabularies sortable
  foreach($vocabularies as $vocabulary){

    if( isset($config_vocab[$vocabulary->machine_name]) ){
      //easy way to object_merge...
      $option_voc[$config_vocab[$vocabulary->machine_name]->order] = (object) array_merge((array)$config_vocab[$vocabulary->machine_name] ,(array)  $vocabulary);
    }
    else{

      $option_voc[$vocabulary->machine_name] =  $vocabulary ;
    }

  }

  ksort($option_voc);


  $no_order=0;
  foreach($option_voc as $vocabulary){
    //$option_voc[$vocabulary->machine_name]=$vocabulary->name;
    $order= isset($vocabulary->order) ? $vocabulary->order : $no_order;
    $form['ekomundi_voc']['rows'][$vocabulary->machine_name]['#data'] = (object) array(
      'vid' => $vocabulary->machine_name,
      'name' => $vocabulary->name,
      'weight' => $order,
    );
   $form['ekomundi_voc']['rows'][$vocabulary->machine_name]['data-'.$vocabulary->machine_name]=array(
      '#type'=>'checkbox',
      '#default_value' => isset($vocabulary->checked) ? $vocabulary->checked : 0,
      //'#title' => $vocabulary->name,
    );
    $form['ekomundi_voc']['rows'][$vocabulary->machine_name]['#weight'] = $order;
    $form['ekomundi_voc']['rows'][$vocabulary->machine_name]['weight-'.$vocabulary->machine_name] = array(
      '#type' => 'textfield',
      '#title' => t('Weight for @title', array('@title' => $vocabulary->name)),
      '#title_display' => 'invisible',
      '#size' => 4,
      '#default_value' => $order,
      '#attributes' => array('class' => array('voc-weight')),
    );
    $no_order++;

  }


  $form = array('packages' => array('#type' => 'vertical_tabs')) + $form;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );


  return $form;
}

function ajax_example_simplest_callback(&$form, $form_state) {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#replace_div_1", render($form['element_to_be_replaced_1'])),
      ajax_command_replace("#replace_div_2", render($form['element_to_be_replaced_2']))
    )
  );
}


function theme_ekomundi_config($vars){
  $form = $vars['form'];

  $rows = array();
  $header = array(t('Vocabulary'),t('Allow'), t('Weight') );
  foreach (element_children($form['ekomundi_voc']['rows']) as $key) {
    $name=$form['ekomundi_voc']['rows'][$key]['#data']->name;

    $row = array();
    $row[] = check_plain($name);
    $row[] = drupal_render($form['ekomundi_voc']['rows'][$key]['data-'.$key]);;

    $row[] = drupal_render($form['ekomundi_voc']['rows'][$key]['weight-'.$key]);


    $rows[] = array('data' => $row, 'class' => array('draggable'));
  }
  //$rows[] = array(array('data' => drupal_render($form['name']) . drupal_render($form['add']), 'colspan' => 4, 'class' => 'edit-name'));
  drupal_add_tabledrag('eko-config-voc', 'order', 'sibling', 'voc-weight');

  $form['ekomundi_voc']['table']=array(
    '#markup'=> theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'eko-config-voc'))),
  );
  $output = drupal_render_children($form);

  return $output;


}

function ekomundi_config_submit($form, $form_state) {
  $voc_config=array();
  foreach($form_state['values'] as $key => $value){
    if (substr($key,0,4)=='eko_'){
      variable_set($key,$value);
    }
    if (substr($key,0,5)=='data-'){
      $voc = str_replace('data-','',$key);

      $voc_config[$voc]=(object) array(
        'checked' => $value,
        'order'=>$form_state['values']['weight-'.$voc],
      );
    }
  }

  variable_set('eko_config_voc',$voc_config);
  drupal_set_message(t('The configuration has been saved.'));
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
