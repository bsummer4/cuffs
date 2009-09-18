#!/bin/sh

# Posts a comment on a message
# usage: post-unfuddle-message.sh project-name id username password messageid
# connent is taken from standard input

project=$1; id=$2; username=$3; password=$4; messageid=$5
body=`cat`

curl -i -u $username:$password -X POST \
  -H 'Accept: application/xml' \
  -H 'Content-type: application/xml' \
  -d "<comment><body>$body</body><body-format>markdown</body-format></comment>" \
  "http://$project.unfuddle.com/api/v1/projects/$id/messages/$messageid/comments"
