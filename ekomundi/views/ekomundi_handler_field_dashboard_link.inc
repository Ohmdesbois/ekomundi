<?php
/**
* Field handler to provide simple renderer that turns a URL into a clickable link.
*/
class ekomundi_handler_field_dashboard_link extends views_handler_field_node {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['nid'] = 'nid';
  }

  function render($values) {
    global $user;
    $value = $this->get_value($values);
    $nid = $this->get_value($values, 'nid');
    //$node = node_load($nid);
    $user_membership = og_get_membership('node',$nid,'user',$user->uid);
    if($user_membership && $user_membership->state==1){
      $img = array(
        'path' => drupal_get_path('theme','themeko').'/images/icones/icone-dashboard.png',
        'alt' => t('Go to the dashboard'),
        'title'=> t('Go to the dashboard'),
        //'attributes' => array('class' => $class)
      );
      
      return l(theme('image', $img), 'node/' . $nid . '/dashboard',array('html' => TRUE));
    }
    else{ 
      return  ;
    }
    
  }

  
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
};
