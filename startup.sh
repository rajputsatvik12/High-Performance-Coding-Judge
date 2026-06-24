sudo systemctl is-active --quiet postgresql || sudo systemctl start postgresql

CGROUP_NAME="myjudge"
CGROUP_PATH="/sys/fs/cgroup/$CGROUP_NAME"

echo "[+] Setting up cgroup: $CGROUP_NAME"

# 1. Ensure controllers are enabled at root (safe even if already set)
echo "[+] Enabling root subtree controllers"
echo "+cpu +memory +pids" | sudo tee /sys/fs/cgroup/cgroup.subtree_control > /dev/null

# 2. Create cgroup if not exists
if [ ! -d "$CGROUP_PATH" ]; then
    sudo mkdir -p "$CGROUP_PATH"
    echo "[+] Cgroup created: $CGROUP_PATH"
else
    echo "[+] Cgroup already exists"
fi

# 3. Fix ownership (your “ownership mess up” fix)
echo "[+] Fixing ownership"
sudo chown -R $USER:$USER "$CGROUP_PATH"

# 4. Ensure subtree control inside myjudge (IMPORTANT for child cgroups later)
echo "[+] Enabling subtree control for myjudge"
echo "+cpu +memory +pids" | sudo tee "$CGROUP_PATH/cgroup.subtree_control" > /dev/null

# 5. Run judge
echo "[+] Starting judge"
JWT_MANAGER_KEY="rvhbveowu422pi3pu$%#%efef23@@#$" ./build/judge_engine