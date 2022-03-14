#!/usr/bin/perl 
# https://perldoc.perl.org/5.8.9/CGI
use CGI;

$query = new CGI;
$name = $query->param(nom);
print $query->header,                    # create the HTTP header
      $query->start_html('hello world,'), # start the HTML
      $query->h1("hello world, $name"),         # level 1 header
      $query->end_html; 
print "nom : ".$query->param(nom)."<br>\n";
print "prenom : ".$query->param(prenom)."<br>\n";
print "mois : ".$query->param(moisnaissance)."<br>\n";
print "sexe : ".$query->param(sexe)."<br>\n";
if ($query->param(majeur) =~ /on/) {
  print "je suis majeur<br>\n";
}
else {
  print "je ne suis pas majeur<br>\n";
}
print "mes commentaires : <ul><pre>".$query->param(commentaire)."</pre></ul>\n";

print "contenu html\n";
