from github import Github, InputGitTreeElement
import base64
import configparser
import re

def read_file_content(file_path):
    with open(file_path, 'r') as file:
        content = file.read()
    return content

def get_version_from_gradle():
    with open('build.gradle', 'r') as f:
        content = f.read()
    match = re.search(r'def VERSION = "(.*?)"', content)
    if match:
        return match.group(1)
    else:
        return None

def get_github_token():
    config = configparser.RawConfigParser()
    with open('local.properties', 'r') as f:
        file_content = '[DEFAULT]\n' + f.read()
    config.read_string(file_content)
    return config.get('DEFAULT', 'githubToken')

def upload_file_to_github(token, repo_name, file_path, commit_message):
    g = Github(token)
    version = get_version_from_gradle()
    repo = g.get_repo(repo_name)
    nunchuk_sdk_repo = g.get_repo("nunchuk-io/nunchuk-android-nativesdk")
    release_note = read_file_content('release_note.md')
    destination_path = "nunchuk-android-nativesdk-arm8-release.aar"
    # Read the file content in binary mode
    with open(file_path, 'rb') as file:
        file_content = file.read()

    # Encode file content to base64
    file_content_base64 = base64.b64encode(file_content).decode()

    # Get the existing file if it exists
    existing_file = repo.get_contents(destination_path) if repo.get_contents(destination_path) else None

    if existing_file:
        # If the file exists, get the SHA hash
        sha = existing_file.sha
        # Determine commit message
        commit_message = "File updated via script"
        # Update the existing file
        repo.update_file(destination_path, commit_message, file_content_base64, sha)
        print(f"File '{file_path}' updated successfully at '{destination_path}' in '{repo_name}' repository.")
    else:
        # Determine commit message
        commit_message = "File uploaded via script"
        # Create a new file
        repo.create_file(destination_path, commit_message, file_content_base64)
        print(f"File '{file_path}' uploaded successfully to '{destination_path}' in '{repo_name}' repository.")

    # Create a release
    repo.create_git_release(version, version, release_note, draft=False)
    nunchuk_sdk_repo.create_git_release(version, version, release_note, draft=False)
    print(f"Release '{version}' created successfully with tag '{version}'.")

# Usage
version = get_version_from_gradle()
github_token = get_github_token()
upload_file_to_github(github_token, "nunchuk-io/nunchuk-android-nativesdk-prebuild", "build/outputs/aar/nunchuk-android-nativesdk-arm8-release.aar", "Upload aar file")