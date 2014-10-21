<?php
/**
* Field handler to provide simple renderer that turns a URL into a clickable link.
*/
class og_hierarchy_handler_field_gid_title extends views_handler_field_node {
  function construct() {
    parent::construct();
    $this->additional_fields['gid'] = 'gid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_entity'] = array('default' => isset($this->definition['link_to_entity default']) ? $this->definition['link_to_entity default'] : FALSE);
    return $options;

  }

  /**
   * Provide link to entity option.
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_entity'] = array(
      '#title' => t('Link this field to the original entity'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_entity']),
    );

    parent::options_form($form, $form_state);
  }



  function render($values) {
    $value = $this->get_value($values);
    $gid = $this->get_value($values, 'gid');
    $node = node_load($gid);

    if (!empty($this->options['link_to_entity'])) {
      return l($node->title, 'node/' . $node->nid ,array('html' => TRUE));
    }

    return $node->title;

  }


  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
};
