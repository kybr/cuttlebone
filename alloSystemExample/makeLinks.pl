#!/usr/bin/env perl
use warnings;
use strict;

-e "../Cuttlebone" or die $!;
-e "../../cuttlebone" or die $!;
-e "../../AlloSystem" or die $!;
-e "../../AlloSystem/alloutil" or die $!;

chdir "../../AlloSystem/alloutil";
qx(ln -s ../../cuttlebone/Cuttlebone .");
