<?php

/**
 * Implementation of hook_views_plugins()
 */
function views_tree_expandable_views_plugins() {
  $plugin = array(
    'style' => array(
      'tree_expandable' => array(
        'title' => t('Tree Expandable'),
        'help' => t('Display the results as a nested tree expandable'),
        'handler' => 'views_tree_expandable_plugin_style_tree',
        'theme' => 'views_tree_expandable',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'parent' => 'list',
        //'path' => drupal_get_path('module', 'views_tree'),
      ),
    ),
  );

  return $plugin;
}

