from github import Github, InputGitTreeElement
import base64

import configparser

def get_github_token():
    config = configparser.RawConfigParser()
    config.read('local.properties')
    return config.get('DEFAULT', 'githubToken')

def upload_file_to_github(token, repo_name, file_path, commit_message):
    g = Github(token)
    repo = g.get_user().get_repo(repo_name)
    with open(file_path, 'rb') as file:
        content = base64.b64encode(file.read())

    # Create a new file in the repository
    git_tree = InputGitTreeElement(file_path, '100644', 'blob', content.decode("utf-8"))
    parent = repo.get_git_commits()[0].sha
    tree = repo.create_git_tree([git_tree], base_tree=parent)
    commit = repo.create_git_commit(commit_message, tree, [parent])
    master_ref = repo.get_git_ref('heads/master')
    master_ref.edit(commit.sha)

# Usage
github_token = get_github_token()
upload_file_to_github(github_token, "nunchuk-android-nativesdk-prebuild", "build/outputs/aar/nunchuk-android-native-arm64_v8a-release.aar", "Upload aar file")