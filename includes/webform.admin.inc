<?php

/**
 * @file
 * Administration pages provided by Webform module.
 */

/**
 * Menu callback for admin/config/content/webform.
 */
function webform_admin_settings() {
  module_load_include('inc', 'webform', 'includes/webform.export');

  $form['components'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available components'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('These are the available field types for your installation of Webform. You may disable any of these components by unchecking its corresponding box. Only checked components will be available in existing or new webforms.'),
  );

  // Add each component to the form:
  $form['components'] += array('#tree' => TRUE);
  $component_types = webform_components(TRUE);
  foreach ($component_types as $key => $component) {
    $form['components'][$key] = array(
      '#title' => $component['label'],
      '#description' => $component['description'],
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => $component['enabled'],
    );
  }

  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default e-mail values'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['email']['webform_default_from_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From address'),
    '#default_value' => webform_variable_get('webform_default_from_address'),
    '#description' => t('The default sender address for emailed webform results. It should match the sending domain, either the website domain or the domain used with the mailing service.'),
    '#required' => TRUE,
  );

  $form['email']['webform_default_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#default_value' => webform_variable_get('webform_default_from_name'),
    '#description' => t('The default sender name which is used along with the default from address.'),
    '#required' => TRUE,
  );

  $form['email']['webform_default_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Default subject'),
    '#default_value' => webform_variable_get('webform_default_subject'),
    '#description' => t('The default subject line of any e-mailed results.'),
  );

  $form['email']['webform_email_html_capable'] = array(
    '#type' => 'checkbox',
    '#title' => t('HTML mail system'),
    '#default_value' => webform_variable_get('webform_email_html_capable'),
    '#description' => t('Whether the mail system configured for webform is capable of sending mail in HTML format.'),
  );

  $form['email']['webform_default_format'] = array(
    '#type' => 'radios',
    '#title' => t('Format'),
    '#options' => array(
      0 => t('Plain text'),
      1 => t('HTML'),
    ),
    '#default_value' => webform_variable_get('webform_default_format'),
    '#description' => t('The default format for new e-mail settings. Webform e-mail options take precedence over the settings for system-wide e-mails configured in MIME mail.'),
    '#states' => array(
      'visible' => array(
        ':input[name="webform_email_html_capable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['email']['webform_format_override'] = array(
    '#type' => 'radios',
    '#title' => t('Format override'),
    '#options' => array(
      0 => t('Per-webform configuration of e-mail format'),
      1 => t('Send all e-mails in the default format'),
    ),
    '#default_value' => webform_variable_get('webform_format_override'),
    '#description' => t('Force all webform e-mails to be sent in the default format.'),
    '#states' => array(
      'visible' => array(
        ':input[name="webform_email_html_capable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['progressbar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Progress bar'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['progressbar']['webform_progressbar_style'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Progress bar style'),
    '#options' => array(
      'progressbar_bar' => t('Show progress bar'),
      'progressbar_page_number' => t('Show page number as number of completed (i.e. Page 1 of 10)'),
      'progressbar_percent' => t('Show percentage completed (i.e. 10%)'),
      'progressbar_pagebreak_labels' => t('Show page labels from page break components'),
      'progressbar_include_confirmation' => t('Include confirmation page in progress bar'),
    ),
    '#default_value' => webform_variable_get('webform_progressbar_style'),
    '#description' => t('Choose how the progress bar should be displayed for multi-page forms.'),
  );
  $form['progressbar']['webform_progressbar_label_first'] = array(
    '#type' => 'textfield',
    '#title' => t('First page label'),
    '#default_value' => webform_variable_get('webform_progressbar_label_first'),
    '#maxlength' => 255,
  );
  $form['progressbar']['webform_progressbar_label_confirmation'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirmation page label'),
    '#default_value' => webform_variable_get('webform_progressbar_label_confirmation'),
    '#maxlength' => 255,
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 20,
  );

  $form['advanced']['webform_tracking_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Track anonymous users by:'),
    '#options' => array(
      'cookie' => t('Cookie only (least strict)'),
      'ip_address' => t('IP address only'),
      'strict' => t('Both cookie and IP address (most strict)'),
    ),
    '#default_value' => webform_variable_get('webform_tracking_mode'),
    '#description' => t('<a href="http://www.wikipedia.org/wiki/HTTP_cookie">Cookies</a> can be used to help prevent the same user from repeatedly submitting a webform. Limiting by IP address is more effective against repeated submissions, but may result in unintentional blocking of users sharing the same address. Confidential submissions are tracked by cookie only. Logged-in users are always tracked by their user ID and are not affected by this option.'),
  );

  $form['advanced']['webform_email_address_format'] = array(
    '#type' => 'radios',
    '#title' => t('E-mail address format'),
    '#options' => array(
      'long' => t('Long format: "Example Name" &lt;name@example.com&gt;'),
      'short' => t('Short format: name@example.com'),
    ),
    '#default_value' => webform_variable_get('webform_email_address_format'),
    '#description' => t('Most servers support the "long" format which will allow for more friendly From addresses in e-mails sent. However many Windows-based servers are unable to send in the long format. Change this option if experiencing problems sending e-mails with Webform.'),
  );

  $form['advanced']['webform_email_address_individual'] = array(
    '#type' => 'radios',
    '#title' => t('E-mailing multiple recipients'),
    '#options' => array(
      '0' => t('Send a single e-mail to all recipients'),
      '1' => t('Send individual e-mails to each recipient'),
    ),
    '#default_value' => webform_variable_get('webform_email_address_individual'),
    '#description' => t('Individual e-mails increases privacy by not revealing the addresses of other recipients. A single e-mail to all recipients lets them use "Reply All" to communicate.'),
  );

  $date_format_options = array();
  foreach (system_get_date_formats() as $type => $type_info) {
    $date_format_options[$type] = t('@label — @sample', array('@label' => $type_info['label'], '@sample' => format_date(REQUEST_TIME, 'custom', webform_date_format($type))));
  }
  $form['advanced']['webform_date_type'] = array(
    '#type' => 'select',
    '#title' => t('Date format'),
    '#options' => $date_format_options,
    '#default_value' => webform_variable_get('webform_date_type'),
    '#description' => t('Choose the format for the display of date components. Only the date portion of the format is used. Reporting and export use the short format.'),
  );

  $form['advanced']['webform_export_use_progressive_batch'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable progressive batch export'),
    '#default_value' => webform_variable_get('webform_export_use_progressive_batch'),
    '#description' => t('This is checked by default. Uncheck if you are experiencing failed export downloads in a load-balanced environment.'),
  );

  $form['advanced']['webform_export_format'] = array(
    '#type' => 'radios',
    '#title' => t('Default export format'),
    '#options' => webform_export_list(),
    '#default_value' => webform_variable_get('webform_export_format'),
  );

  $form['advanced']['webform_csv_delimiter'] = array(
    '#type' => 'select',
    '#title' => t('Default export delimiter'),
    '#description' => t('This is the delimiter used in the CSV/TSV file when downloading Webform results. Using tabs in the export is the most reliable method for preserving non-latin characters. You may want to change this to another character depending on the program with which you anticipate importing results.'),
    '#default_value' => webform_variable_get('webform_csv_delimiter'),
    '#options' => array(
      ','  => t('Comma (,)'),
      '\t' => t('Tab (\t)'),
      ';'  => t('Semicolon (;)'),
      ':'  => t('Colon (:)'),
      '|'  => t('Pipe (|)'),
      '.'  => t('Period (.)'),
      ' '  => t('Space ( )'),
    ),
  );

  $form['advanced']['webform_export_wordwrap'] = array(
    '#type' => 'radios',
    '#title' => t('Export word-wrap'),
    '#options' => array(
      '0' => t('Only text containing return characters'),
      '1' => t('All text'),
    ),
    '#default_value' => webform_variable_get('webform_export_wordwrap'),
    '#description' => t('Some export formats, such as Microsoft Excel, support word-wrapped text cells.'),
  );

  $form['advanced']['webform_submission_access_control'] = array(
    '#type' => 'radios',
    '#title' => t('Submission access control'),
    '#options' => array(
      '1' => t('Select the user roles that may submit each individual webform'),
      '0' => t('Disable Webform submission access control'),
    ),
    '#default_value' => webform_variable_get('webform_submission_access_control'),
    '#description' => t('By default, the configuration form for each webform allows the administrator to choose which roles may submit the form. You may want to allow users to always submit the form if you are using a separate node access module to control access to webform nodes themselves.'),
  );

  $form['advanced']['webform_token_access'] = array(
    '#type' => 'radios',
    '#title' => t('Token access'),
    '#options' => array(
      '1' => t('Allow tokens to be used in Webforms.'),
      '0' => t('Disable tokens in Webforms'),
    ),
    '#default_value' => webform_variable_get('webform_token_access'),
    '#description' => t('Tokens can be used to reveal sensitive information. Allow tokens if Webform creators are trusted.'),
  );

  $form['advanced']['webform_email_select_max'] = array(
    '#type' => 'textfield',
    '#title' => t("Select email mapping limit"),
    '#default_value' => webform_variable_get('webform_email_select_max'),
    '#description' => t('When mapping emails addresses to a select component, limit the choice to components with less than the amount of options indicated. This is to avoid flooding the email settings form.'),
  );

  $form['advanced']['webform_fieldset_wrap'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use fieldsets for grouped components'),
    '#default_value' => webform_variable_get('webform_fieldset_wrap'),
    '#description' => t('Components containing multiple fields, such as checkboxes and radio buttons, will be wrapped in fieldsets. This improves the accessibility of webforms and helps them conform to web standards, but may require updates to the CSS of the active theme.'),
  );

  $form['#theme'] = 'webform_admin_settings';
//   array_unshift($form['#submit'], 'webform_admin_settings_submit');
  $form['#submit'][] = 'webform_admin_settings_submit';

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit handler for the webform_admin_settings() form.
 */
function webform_admin_settings_submit($form, &$form_state) {
  $disabled_components = array();
  foreach ($form_state['values']['components'] as $name => $enabled) {
    if (!$enabled) {
      $disabled_components[] = $name;
    }
  }
  // Update $form_state and let system_settings_form_submit() handle saving.
  $form_state['values']['webform_disabled_components'] = $disabled_components;
  unset($form_state['values']['components']);

  // Trim out empty options in the progress bar options.
  $form_state['values']['webform_progressbar_style'] = array_keys(array_filter($form_state['values']['webform_progressbar_style']));
  $config = config('webform.settings');
  foreach ($form_state['values'] as $key => $value) {
    if (substr($key, 0, 8) === 'webform_') {
      $config->set($key, $value);
    }
  }
  $config->save();
  backdrop_set_message(t('The configuration options have been saved.'));
}

/**
 * Theme the output of the webform_admin_settings() form.
 */
function theme_webform_admin_settings($variables) {
  $form = $variables['form'];

  // Format the components into a table.
  foreach (element_children($form['components']) as $key) {
    $row = array();
    $row[] = $form['components'][$key]['#title'];
    $row[] = $form['components'][$key]['#description'];
    $form['components'][$key]['#title'] = NULL;
    $form['components'][$key]['#description'] = NULL;
    $row[] = array('data' => backdrop_render($form['components'][$key]), 'align' => 'center');
    $rows[] = $row;
  }
  $header = array(t('Name'), t('Description'), array('data' => t('Enabled'), 'class' => array('checkbox')));

  // Create the table inside the form.
  $form['components']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return backdrop_render_children($form);
}

/**
 * Menu callback for admin/content/webform. Displays all webforms on the site.
 */
function webform_admin_content() {
  // Determine whether the webforms listing will be provided by Views or a
  // hard-coded table.
  if (!webform_variable_get('webform_table')) {
    $view = views_get_view('webform_webforms');

    // Programmatically add the empty text if there are no webforms available.
    // This is the same text that's generated if a hard-code table is used.
    if (!$view->disabled) {
      if (empty($view->result)) {
        $empty_content = _webform_admin_empty_text();
        $options = array(
          'id' => 'area',
          'table' => 'views',
          'field' => 'area',
          'empty' => TRUE,
          'content' => $empty_content,
          'tokenize' => 0,
        );
        $view->set_item('default', 'empty', 'area', $options);
      }

      return $view->preview('default');
    }
  }

  $header = array(
    array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Created'),
      'field' => 'n.created',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Status'),
      'field' => 'w.status',
    ),
    array(
      'data' => t('View'),
    ),
    array(
      'data' => t('Operations'),
    ),
  );

  $nodes_per_page = 25;

  $query = db_select('webform', 'w')->extend('PagerDefault')->extend('TableSort');
  $query->join('node', 'n', 'w.nid = n.nid');
  $query->fields('w');
  $query->fields('n');
  $query->limit($nodes_per_page);
  $query->orderByHeader($header);
  $result = $query->execute()->fetchAllAssoc('nid');
  // The db query fetches stdClass objects, which causes issues with tasks like
  // trying to use node_access() on them. Load the actual Node objects instead.
  $nodes = node_load_multiple(array_keys($result));

  return theme('webform_admin_content', array('nodes' => $nodes, 'header' => $header));
}

/**
 * Create a comma-separated list of content types that are webform-enabled.
 *
 * @see _webform_admin_empty_text()
 */
function webform_admin_type_list() {
  $webform_types = webform_node_types();
  $webform_type_list = '';
  $webform_type_count = count($webform_types);
  foreach ($webform_types as $n => $type) {
    $webform_type_list .= l(node_type_get_name($type), 'node/add/' . str_replace('_', '-', $type));
    if ($n + 1 < $webform_type_count) {
      $webform_type_list .= $webform_type_count == 2 ? ' ' : ', ';
    }
    if ($n + 2 == $webform_type_count) {
      $webform_type_list .= t('or') . ' ';
    }
  }

  return $webform_type_list;
}

/**
 * The text to be shown in the webforms listing when there are no webforms.
 *
 * This is used when the webforms listing is provided either by Views or by a
 * hard-coded table.
 *
 * @see webform_admin_type_list()
 * @see webform_admin_content()
 * @see theme_webform_admin_content
 */
function _webform_admin_empty_text() {
  $webform_types = webform_node_types();
  if (empty($webform_types)) {
    $message = t('Webform is currently not enabled on any content type.') . ' ' . t('Visit the <a href="!url">Webform settings</a> page and enable Webform on at least one content type.', array('!url' => url('admin/config/content/webform')));
  }
  else {
    $webform_type_list = webform_admin_type_list();
    $message = t('There are currently no webforms on your site. Create a !types piece of content.', array('!types' => $webform_type_list));
  }
  return $message;
}

/**
 * Generate a list of all webforms available on this site.
 *
 * This is used as a fallback, in cases like when the default view that provides
 * the webform listing is disabled.
 */
function theme_webform_admin_content($variables) {
  $nodes = $variables['nodes'];
  $header = $variables['header'];

  $rows = array();
  foreach ($nodes as $node) {
    $row = array();

    $row[] = array(
      'data' => l($node->title, 'node/' . $node->nid),
      'class' => array('webform-titles'),
      'data-label' => t('Title'),
    );
    $row[] = array(
      'data' => format_date($node->created, 'short'),
      'class' => array('webform-created'),
      'data-label' => t('Created'),
    );
    $row[] = array(
      'data' => $node->webform['status'] ? t('Open') : t ('Closed'),
      'class' => array('webform-status'),
      'data-label' => t('Status'),
    );
    $row[] = array(
      'data' => array(
        '#type' => 'operations',
        '#links' => _webform_admin_get_webform_view_links($node),
      ),
      'class' => array('webform-view'),
      'data-label' => t('View'),
    );
    $row[] = array(
      'data' => array(
        '#type' => 'operations',
        '#links' => _webform_admin_get_webform_operations($node),
      ),
      'class' => array('webform-operations'),
      'data-label' => t('Operations'),
    );

    $rows[] = $row;
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => _webform_admin_empty_text())) . theme('pager');
}

/**
 * Given a webform, return a list of links for its various "Results" sub-pages.
 */
function _webform_admin_get_webform_view_links(Node $node) {
  $links = array();

  $links['submissions'] = array(
    'title' => t('Submissions'),
    'href' => 'node/' . $node->nid . '/webform-results',
  );
  $links['analysis'] = array(
    'title' => t('Analysis'),
    'href' => 'node/' . $node->nid . '/webform-results/analysis',
  );
  $links['table'] = array(
    'title' => t('Table'),
    'href' => 'node/' . $node->nid . '/webform-results/table',
  );
  $links['download'] = array(
    'title' => t('Download'),
    'href' => 'node/' . $node->nid . '/webform-results/download',
  );

  return $links;
}

/**
 * Given a webform, return a list of operations that can be performed on it.
 */
function _webform_admin_get_webform_operations(Node $node) {
  $links = array();

  if (node_access('update', $node)) {
    $links['edit'] = array(
      'title' => t('Edit'),
      'href' => 'node/' . $node->nid . '/edit',
    );
    $links['components'] = array(
      'title' => t('Components'),
      'href' => 'node/' . $node->nid . '/webform',
    );
  }
  if (user_access('delete all webform submissions')) {
    $links['clear'] = array(
      'title' => t('Clear'),
      'href' => 'node/' . $node->nid . '/webform-results/clear',
    );
  }

  return $links;
}
