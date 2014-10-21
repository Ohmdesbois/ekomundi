<?php
/**
 * @file
 * Contains the numeric argument handler.
 */

/**
 * This handle will retrieve other groups in the hierarchy from a entity ID
 *
 * @ingroup views_argument_handlers
 */
class og_hierarchy_handler_argument_gid extends views_handler_argument_numeric {
  function option_definition() {
    $options = parent::option_definition();

    $options['build_directions'] =  array('default' => array('none' => 'none'));
    $options['include_argument'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Remove unwanted options
    unset($form['not']);

    $form['build_directions'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Build Direction'),
      '#description' => t('Retrieve groups in the direction specified using the argument(s) as a starting point. If "none" are checked "parent" and "children" will not work'),
      '#required' => TRUE,
      '#options' => array(
        'none' => t('None'),
        'parents' => t('Parents'),
        'children' => t('Children'),
      ),
      '#default_value' => $this->options['build_directions'],
    );

    $form['include_argument'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include the argument'),
      '#description' => t('If selected, the group(s) specified in the argument(s) will be added to the output. Works only if "parent" or "children" are selected.'),
      '#default_value' => $this->options['include_argument'],
    );
    
  }

  function get_subgroup_tree() {
    $tree = array();
    $directions = array_filter($this->options['build_directions']);
    $arguments = explode(',', $this->argument);
    $entity_type = "node";
    if (preg_match("/user\/[0-9]?/",$_GET['q'])) {
      $entity_type = "user";
    }
    if (preg_match("/node\/[0-9]?/",$_GET['q'])) {
      $entity_type = "node";
    }
    // Get all group IDs that are in the argument's family
    foreach ($arguments as $argument) {
      foreach ($directions as $direction) {
        // Fetch the entities
        $entities = og_get_hierarchy($entity_type, $argument, array('nested' => FALSE, 'entity_type' => array($entity_type),'direction' => $direction));

        // Add the entities to an array
        foreach ($entities as $entity) {
          if (isset($entity->etid))
            $tree[] = $entity->etid;
        }
      }
    }

    return !empty($tree) ? $tree : array(-1);
  }

  function query($group_by = FALSE) {
    $this->ensure_my_table();
    if($this->options['build_directions'] != 'none'){

      if (!empty($this->options['break_phrase'])) {
        views_break_phrase($this->argument, $this);
      }
      else {
        $this->value = array($this->argument);
      }

      $tree = $this->get_subgroup_tree();
      $arguments = explode(',', $this->argument);

      if ($this->options['include_argument']) {
        $tree = array_merge($tree, $arguments);
      }
      $tree = array_unique($tree);
      $placeholders = implode(', ', array_fill(0, sizeof($tree), '%d'));
      $base_table = 'og_membership_' . $this->query->base_table;
      $base_field = 'gid';
      $this->query->add_where(0, "$base_table.$base_field", $tree,"IN");
    }
  }
}
