<?php

const OBJ_COUNT = 1000;

echo 'Creating loads of objects...' . PHP_EOL;

for ($i = 0; $i < OBJ_COUNT; $i++) {
    $c = new Cereal();
}

for ($i = 0; $i < OBJ_COUNT; $i++) {
    $c = get_cereal();
}

echo 'Done.' . PHP_EOL;
