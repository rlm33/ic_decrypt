require 'benchmark'

decrypter = "./dist/Debug/GNU-Linux-x86/ic_decrypt"
Benchmark.bm do |x| 
	puts 'DOWNCASE:'
	x.report{system("#{decrypter} alfa beta gamma -d downcase")}
	puts 'ALPHANUMERIC'
	x.report{system("#{decrypter} alf4 b3ta gaMMa -d alphanumeric")}
	puts 'COMPLETE'
	x.report{system("#{decrypter} a:@~ -d complete")}
end