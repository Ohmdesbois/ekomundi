<?php

/**
 * @file
 * Provides Color integration on behalf of node.module.
 */

/**
 * Implements hook_colors_info().
 */
function colors_node_title_colors_info() {
  return array(
    'colors_node_title_type' => array(
      'title' => 'Node title type',
      'short_description' => t('Enable the title colors for node types'),
      'long_description' => t('Colors for node types. If enabled, you may set colors for each node type below.'),
      'function' => 'node_type_get_names',
    ),
  );
}

/**
 * Implements hook_colors_classes().
 *
 * Provide colors per node type.
 */
function colors_node_title_colors_classes($entity) {
  $class_names = array();
  if (variable_get('colors_node_title_type_enabled', FALSE)) {
    if ($entity->entity_type == 'node') {
      $class_names[] = 'colors-node-title-type-' . $entity->type;
    }
  }
  return $class_names;
}
