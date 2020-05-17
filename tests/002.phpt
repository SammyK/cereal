--TEST--
Test arena functionality
--SKIPIF--
<?php if (!extension_loaded('cereal')) die('skip: cereal extension required'); ?>
--INI--
cereal.use_arena=1
--FILE--
<?php
echo 'Where are my Lucky Charms?' . PHP_EOL;
$cereal = new Cereal();
$cereal->name = 'Lucky Charms';
var_dump($cereal);

$cereal2 = get_cereal();
$cereal2->name = 'Cinnamon Toast Crunch';
$cereal2->milk = 'almond';
var_dump($cereal2);

function userlandCereals() {
	foreach (range(0, 3) as $i) {
		$c = new Cereal();
		echo 'Made userland cereal #' . spl_object_id($c) . PHP_EOL;
	}
}

function internalCereals() {
	foreach (range(0, 3) as $i) {
		$c = get_cereal();
		echo 'Made internal cereal #' . spl_object_id($c) . PHP_EOL;
	}
}

userlandCereals();
internalCereals();
?>
--EXPECTF--
Where are my Lucky Charms?
object(Cereal)#1 (2) {
  ["name"]=>
  string(12) "Lucky Charms"
  ["milk"]=>
  string(0) ""
}
object(Cereal)#2 (2) {
  ["name"]=>
  string(21) "Cinnamon Toast Crunch"
  ["milk"]=>
  string(6) "almond"
}
Made userland cereal #3
Made userland cereal #4
Made userland cereal #5
Made userland cereal #6
Made internal cereal #7
Made internal cereal #8
Made internal cereal #9
Made internal cereal #10
Freeing arena (%s)
