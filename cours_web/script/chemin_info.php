<?php
// Demandez à l'utilisateur de saisir un chemin
// $customPath = readline("Veuillez saisir un chemin personnalisé : ");
// connexion BD
function connectionBD()
{
    include('config.php');
    try {
        $bdd = new PDO('mysql:host=' . $host . ';dbname=' . $db . ';charset=utf8', '' . $user . '', '' . $password . '');
        $bdd->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    } catch (PDOException $e) {
        die('Erreur de connexion : ' . $e->getMessage() . " à la ligne " . $e->getLine());
    }
    return $bdd;
}
// fonction message 
function messageAlert($alertMes, $urlNav)
{
    echo '<div style="position: fixed; top: 40%; left: 30%; background-color: #E7F7FF; padding: 13px; border-radius: 10px;">';
    echo '<h3> Message </h3>';
    echo '<span class="separator"></span>';
    echo '<h4 class="text-center" style="position: relative; display: flex; flex-direction: column; width: 100%;">' . $alertMes . '</h4>';
    echo '</div>';
    header("Refresh: 3; url=$urlNav");
    exit();
}
// formater les taille de chaque fichiers 
function formatSizeUnits($bytes)
{
    $units = array('B', 'KB', 'MB', 'GB', 'TB');
    $i = 0;
    while ($bytes >= 1024 && $i < count($units) - 1) {
        $bytes /= 1024;
        $i++;
    }
    return round($bytes, 2) . ' ' . $units[$i];
}

$result = [];
// print_r($result);
// recuperer le chemin depuis front avec methode post
if (!empty($_POST['chemin'])) {
    $custumPath = $_POST['chemin'];
    $pathAttendu = "C:\\Users\\Amir\\Desktop\\";
    // echo "custum path :", $custumPath, "\n";
    function getDirectory($path, $result)
    {
        // Utilisez le chemin personnalisé comme répertoire courant
        chdir($path);
        // Obtient le répertoire courant
        $dir = getcwd();
        // Obtient la liste des fichiers et répertoires dans le répertoire courant
        $items = scandir($dir);
        foreach ($items as $key => $item) {
            if ($item !== '.' && $item !== '..') {
                $path = $dir . '/' . $item;
                // si c'est un fichier
                if (is_file($path)) {
                    $type = "FICHIER";
                    $ext = pathinfo($item, PATHINFO_EXTENSION);
                    $ext = strtolower($ext);
                    $size = filesize($path);
                    $formatSize = formatSizeUnits($size);
                    $fileInfo = array(
                        // 'id' => $key - 1,
                        'Type' => $type,
                        'NomFich' => $item,
                        'CheminF' => $path,
                        'Ext' => $ext,
                        'Taille' => $formatSize
                    );
                    // si c'est un dossier
                    $result[] = $fileInfo;
                } elseif (is_dir($path)) {
                    $folderName = basename($path);
                    if (substr($folderName, 0, 1) !== '.') {
                        $fileInfo = array(
                            "NomDoss" => $item,
                            "Type" => "DOSSIER",
                            "Children" => getDirectory($path, [])
                        );
                        $result[] = $fileInfo;
                    }
                }
                // Affiche le type (FICHIER ou DOSSIER)
                // echo "$type : $item\n";
            }
        }
        return $result;
    }
    // Vérifiez si le chemin existe et est un répertoire et qu'il respecte le chemin principal
    if (is_dir($custumPath) && strpos($custumPath, $pathAttendu) === 0) {
        // Appelle la fonction pour explorer récursivement le chemin
        $result = getDirectory($custumPath, $result);
        // envoie une message en cas d'erreur 
    } else {
        messageAlert("Le chemin n'existe pas ou il y a une erreur de syntaxe.</br> Le chemin attendu doit contenir (C:\Users\Amir\Desktop\) </br> Merci !", "index.php");
    }
}
