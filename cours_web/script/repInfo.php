<?php
$dir = getcwd();

if (!is_dir('myBot')) {
    mkdir('myBot');
}

// Parcours tous les fichiers dans le répertoire courant
$files = scandir($dir);
foreach ($files as $file) {
    if ($file !== '.' && $file !== '..' && is_file($file)) {
        $ext = pathinfo($file, PATHINFO_EXTENSION);
        $ext = strtolower($ext);
        // Ignorer les fichiers avec certaines extensions
        if ($ext != 'bat' && $ext != 'zip' && $ext != 'rar') {
            // Crée un répertoire pour l'extension si nécessaire
            if (!is_dir("myBot/$ext")) {
                mkdir("myBot/$ext");
            }
            // Déplace le fichier dans le répertoire correspondant
            rename($file, "myBot/$ext/$file");
        }
    }
}
// Parcours tous les répertoires dans le répertoire courant
foreach ($files as $file) {
    if ($file !== '.' && $file !== '..' && is_dir($file)) {
        $zipFile = $file . '.zip';
        $rarFile = $file . '.rar';
        // Vérifie si un fichier ZIP ou RAR portant le nom du répertoire existe
        if (file_exists($zipFile)) {
            // Crée un répertoire "zip" s'il n'existe pas
            if (!is_dir('zip')) {
                mkdir('zip');
            }
            // Déplace le répertoire et le fichier ZIP correspondant
            rename($file, "zip/$file");
            rename($zipFile, "zip/$zipFile");
        } elseif (file_exists($rarFile)) {
            // Crée un répertoire "rar" s'il n'existe pas
            if (!is_dir('rar')) {
                mkdir('rar');
            }
            // Déplace le répertoire et le fichier RAR correspondant
            rename($file, "rar/$file");
            rename($rarFile, "rar/$rarFile");
        }
    }
}

echo "Done!";
