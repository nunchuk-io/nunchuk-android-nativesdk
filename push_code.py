import os
import subprocess

def merge_and_push():
    # Set up paths and branch names
    master_branch = 'master'
    main_branch = 'main'
    commit_message = 'Merge changes from master branch'

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
    subprocess.run(['git', 'merge', '--no-ff', '-m', commit_message, 'origin/master'])

    # Push changes to the main branch on GitHub
    subprocess.run(['git', 'push', 'origin-github', main_branch])

merge_and_push()