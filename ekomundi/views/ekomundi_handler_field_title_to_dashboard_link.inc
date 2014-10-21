<?php
/**
* Field handler to provide simple renderer that turns a URL into a clickable link.
*/
class ekomundi_handler_field_title_to_dashboard_link extends views_handler_field_node {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['title'] = 'title';
  }

  function render($values) {
    global $user;
    $value = $this->get_value($values);
    $nid = $this->get_value($values, 'nid');
    $title = $this->get_value($values, 'title');
    //$node = node_load($nid);
    //$user_membership = og_get_membership('node',$nid,'user',$user->uid);
    return l($title, 'node/' . $nid . '/dashboard',array('html' => TRUE));
    /*if($user_membership && $user_membership->state==1){
      return l($title, 'node/' . $nid . '/dashboard',array('html' => TRUE));
    }
    else{
      return l($title, 'node/' . $nid ,array('html' => TRUE)); ;
    }*/

  }


  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
};
