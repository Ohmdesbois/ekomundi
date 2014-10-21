<?php
/**
* @file
* Provide views data and handlers for statistics.module
*/
/**
* @defgroup views_statistics_module statistics.module handlers
*
* Includes the ability to create views of just the statistics table.
* @{
*/

/**
* Implements hook_views_data()
*/
function ekomundi_views_data() {
  $data['node']['ekomundi_ict'] = array(
    'title' => t('Icone content type'), // The item it appears as on the UI,
    'help' => t('Provide an icone content type.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'ekomundi_handler_field_icone_content_type',
    ),
  );
  $data['node']['ekomundi_dashboard_link'] = array(
    'title' => t('Dashboard link'), // The item it appears as on the UI,
    'help' => t('Provide a link to dashboard.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'ekomundi_handler_field_dashboard_link',
    ),
  );
  $data['node']['ekomundi_dashboard_link'] = array(
    'title' => t('Title to dashboard'), // The item it appears as on the UI,
    'help' => t('Provide a controled link to dashboard.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'ekomundi_handler_field_title_to_dashboard_link',
    ),
  ); 
  return $data;
}


function _ekomundi_icone_content_type( $node, $class = 'icone-type-contenu', $with_style = FALSE) {
  $icone_teaser = false;
  switch( $node->type){
    case 'action' :
      $field=field_get_items('node', $node, 'field_type_groupe_travail' );
      $term=taxonomy_term_load($field[0]['tid']);
      $icone_teaser = drupal_strtolower($term->name) ;
      break;
    default :
      $icone_teaser = $node->type;
      break;

  }

  if($icone_teaser){
    $img = array(
      'path' => drupal_get_path('theme','themeko').'/images/icones/icone-'.$icone_teaser.'.png',
      'alt' => t(str_replace('_',' ',$icone_teaser)),
      'title'=> t(ucfirst(str_replace('_',' ',$icone_teaser))),
      'attributes' => array('class' => $class)
    );




    if($with_style){
      $img['style_name'] = 'icone_content_type';
      $img['height'] = NULL;
      $img['width'] = NULL;
      return theme_image_style($img);
      //return theme('image_style',$img);
    }
    else{
      return theme('image', $img);
    }
  }
}
