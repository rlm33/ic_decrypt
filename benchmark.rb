require 'benchmark'

$decrypter = "./dist/Debug/GNU-Linux-x86/ic_decrypt"

$to_test = "alpha gamma light zoned"

def ic_decrypt to_test, domain, numthreads=nil
	system("#{$decrypter} #{to_test} -d #{domain} #{"-p -n #{numthreads}" if numthreads} > log_benchmark.txt")
end

def benchmark to_test, domain_name, domain_size
	Benchmark.bm do |x| 
		puts domain_name
		x.report("Simple:\n") { ic_decrypt(to_test, domain_name) }
		x.report("2 threads:\n") { ic_decrypt(to_test, domain_name, 2) }
		x.report("3 threads:\n") { ic_decrypt(to_test, domain_name, 3) }
		x.report("4 threads:\n") { ic_decrypt(to_test, domain_name, 4) }
		x.report("#{$downcase_size} threads\n:") { ic_decrypt(to_test, domain_name, domain_size) }
	end
end

benchmark $to_test, 'DOWNCASE', 26