<?php

/**
 * @file
 * Developed by Gabor Szanto.
 *  hello@szantogabor.com
 *  http://szantogabor.com
 */
class PgmMemcache extends MemCacheDrupal {
  private $lastId;
  private $lastArray;

  /**
   * @param string $bin
   */
  public function __construct($bin) {
    $this->lastArray = variable_get('pgm_cache_debug_module_implements_cache_saved', FALSE);
    parent::__construct($bin);
  }

  /**
   * @inheritdoc
   */
  public function get($cid) {
    $return = parent::get($cid);

    if ($this->lastArray
      && variable_get('pgm_cache_debug_enable_log', FALSE)
      && $cid == 'module_implements'
      && $return != $this->lastArray
    ) {
      $log = new stdClass();
      $log->data = array_diff($return, $this->lastArray);
      $log->log = '';
      $log->timestamp = REQUEST_TIME;
      $log->readwrite = 'read';

      pgm_cache_debug_save($log);

      if (pgm_cache_debug_preserve_cache()) {
        $this->set('module_implements', $this->lastArray);
        return $this->lastArray;
      }
    }

    return $return;
  }

  /**
   * @inheritdoc
   */
  public function set($cid, $data, $expire = CACHE_PERMANENT) {
    if ($cid == 'module_implements' && !$this->lastArray) {
      variable_set('pgm_cache_debug_module_implements_cache_saved', $data);
    }

    if ($this->lastArray
      && variable_get('pgm_cache_debug_enable_log', FALSE)
      && $cid == 'module_implements'
      && $data != $this->lastArray
    ) {
      $log = new stdClass();
      $log->data = array_diff($data, $this->lastArray);
      $log->log = '';
      $log->timestamp = REQUEST_TIME;
      $log->readwrite = 'read';

      pgm_cache_debug_save($log);

      if (pgm_cache_debug_preserve_cache()) {
        $data = $this->lastArray;
      }
    }
    parent::set($cid, $data, $expire);
  }
}
