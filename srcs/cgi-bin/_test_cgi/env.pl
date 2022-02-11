#!/usr/bin/env perl

# https://gilles-hunault.leria-info.univ-angers.fr/selfhtml_fr/cgiperl/introduction/variablesenvironnement.htm

# commande bash : perl .pl

print "Cgi env webserver perl : \r\n\r\n";
for my $var (sort keys %ENV)
{
    printf "%s=\"%s\"\n", $var, $ENV{$var};
}
