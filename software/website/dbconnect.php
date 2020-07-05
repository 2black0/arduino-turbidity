<?php
  DEFINE ('DBUSER', 'root');
  DEFINE ('DBPW',   '');
  DEFINE ('DBHOST', 'localhost');
  DEFINE ('DBNAME', 'turbidity');
 		
  $dbc = mysqli_connect(DBHOST,DBUSER,DBPW);
  if (!$dbc) 
  {
    die("Koneksi ke database gagal dilakukan: " . mysqli_error($dbc));
    exit();
  }
	 
  $dbs = mysqli_select_db($dbc, DBNAME);
  if (!$dbs) 
  {
    die("Nama database tidak dikenal: " . mysqli_error($dbc));
    exit();
  }
?>
