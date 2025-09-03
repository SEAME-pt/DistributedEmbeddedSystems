#!/bin/bash
set -e
docker buildx build --platform linux/arm64 --progress=plain --load -t  cross-env --build-arg password=$(grep password .env | cut -d '=' -f2) --build-arg user=$(grep user .env | cut -d '=' -f2) .


if [ -f "./digital_cluster" ]; then

    echo "Removing existing executable from local machine"

    rm ./digital_cluster

fi


echo "Copying new executable from Docker container"


docker cp $(docker create cross-env):/workspace/build/digital_cluster ./digital_cluster

echo "Removing existing executable from JetRacer"


ssh jetracer@100.123.70.46 "rm -f /home/jetracer/qt_cluster/build/digital_cluster"


echo "Copying new executable to Raspberry"

scp ./digital_cluster jetracer@100.123.70.46:/home/jetracer/qt_cluster/build

ssh jetracer@100.123.70.46 "cd qt_cluster && docker-compose build && docker-compose up"
