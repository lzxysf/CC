read YES_OR_NO

case "$YES_OR_NO" in
YES|yes|y|Yes)
    echo "yes";;
NO|no|n|No)
    echo "no";;
*)
    echo "input other"
esac