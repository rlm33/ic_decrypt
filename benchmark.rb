require 'benchmark'

decrypter = "./dist/Debug/GNU-Linux-x86/ic_decrypt"
Benchmark.bm do |x| 
	#x.report{system("#{decrypter} alfa")}
	#x.report{system("#{decrypter} beta")}
	x.report{system("#{decrypter} gamma")}
end