import os

def find_todos_in_file(filepath):
    todos = []
    with open(filepath, 'r', encoding='utf-8') as file:
        for lineno, line in enumerate(file, start=1):
            if '@TODO' in line:
                print(f'Found TODO in {filepath} at line {lineno}: {line.strip()}')
                todos.append((lineno, line.strip()))
    return todos

def search_codebase(root_dir, exclude_dirs=None):
    if exclude_dirs is None:
        exclude_dirs = []
    tasks = {}
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if any(exclude_dir in dirpath for exclude_dir in exclude_dirs):
                continue
            if filename.endswith(('.cpp', '.h', '.hpp', '.c', '.cc', '.cxx', '.tpp', '.ipp')):
                filepath = os.path.join(dirpath, filename)
                print(f'Inspecting file: {filepath}')
                todos = find_todos_in_file(filepath)
                if todos:
                    tasks[filepath] = todos
                    print(f'Added {len(todos)} TODOs from {filepath}')
    return tasks

def write_tasks_to_file(tasks, output_file):
    output_dir = os.path.dirname(output_file)
    if not os.path.exists(output_dir):
        print(f'Creating directory: {output_dir}')
        os.makedirs(output_dir)
    
    with open(output_file, 'w', encoding='utf-8') as file:
        for filepath in sorted(tasks):
            file.write(f'## {filepath}\n')
            for lineno, todo in tasks[filepath]:
                file.write(f'- Line {lineno}: {todo}\n')
            file.write('\n')
            print(f'Wrote TODOs from {filepath} to {output_file}')

if __name__ == '__main__':
    root_directory = '.'  # Change this to your codebase root directory
    output_file = 'deploy/tasks/tasks.md'
    exclude_dirs = ["dependencies"]
    tasks = search_codebase(root_directory, exclude_dirs)
    if tasks:
        print(f'Found TODOs in {len(tasks)} files')
        write_tasks_to_file(tasks, output_file)
        print(f'TODOs have been written to {output_file}')
    else:
        print('No TODOs found.')
