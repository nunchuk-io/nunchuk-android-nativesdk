import os
import subprocess

def merge_and_push():
    # Set up paths and branch names
    master_branch = 'master'
    main_branch = 'main'

    # Fetch latest changes from GitLab
    subprocess.run(['git', 'fetch', 'origin'])

    # Checkout the master branch
    subprocess.run(['git', 'checkout', master_branch])

    # Pull latest changes from GitLab
    subprocess.run(['git', 'pull', 'origin', master_branch])

    # Fetch latest changes from GitHub
    subprocess.run(['git', 'fetch', 'origin-github'])

    # Checkout the main branch
    subprocess.run(['git', 'checkout', main_branch])

    # Merge the master branch into the main branch
    subprocess.run(['git', 'merge', 'origin/master'])


merge_and_push()