<?php
/**
* Field handler to provide simple renderer that turns a URL into a clickable link.
*/
class ekomundi_handler_field_icone_content_type extends views_handler_field_node {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['nid'] = 'nid';
  }

  function render($values) {
    $value = $this->get_value($values);
    $nid = $this->get_value($values, 'nid');
    $node = node_load($nid);
    return _get_icone_content_type($node,$node->type,'views-content-type-icone');
  }

  
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
};
