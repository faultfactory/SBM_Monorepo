import subprocess
out = subprocess.check_output('arduino-cli board list',shell=True)
if len(str(out).split('\\n')) > 2:
	print(str(out).split('\\n')[1].split(' ')[0])
else:
	print('')